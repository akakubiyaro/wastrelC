/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	App.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

#define NUM_SIMULTANEOUS_SOUNDS 64

// Calculate the frame delta target based on the FPS target.
static const double kDeltaTarget = (1.0 / kFPSTarget) * 1000.0;

// Other variables to keep track of time between frames.
static u64 FPSStart = 0;
static u64 FPSFrameCount = 0;
static u64 FPS = 0;
static u64 BeginTicks = 0;
static u64 FrameCount = 0;

// The current time delta.
static double Delta = 0.0;

// Current fixed time delta.
static double FixedDelta = 1.0 / kFPSTarget;

// viewport rectangle.
static SDL_Rect CurAppViewport = { 0, 0, 0, 0 };

// the SDL renderer.
static SDL_Renderer *CurAppRenderer = NULL;

// the SDL window.
static SDL_Window *CurAppWindow = NULL;

// Current display index.
static int CurDisplayIndex = -1;

// SDL app settings.
static AppSettings CurAppSettings = (AppSettings) {
	.title = "smol v0.1a",
#if defined(__APPLE__)
	.driverHint = "metal",
#elif defined(_WIN32) || defined(_WIN64) 
	.driverHint = "direct3d",
#else
	.driverHint = NULL,
#endif
	.width = 800,
	.height = 600,
	.fullscreen = false,
	.vSync = true,
	.escapeToQuit = true
};

static AppCallbacks CurAppCallbacks = (AppCallbacks) {
	.OnInit = NULL,
	.OnKeyUp = NULL,
	.OnKeyDown = NULL,
	.OnMouseUp = NULL,
	.OnMouseDown = NULL,
	.OnOtherEvent = NULL,
	.OnUpdate = NULL,
	.OnDraw = NULL,
	.OnShutdown = NULL
};

#define FireCallback(func, ...) if(CurAppCallbacks.func) CurAppCallbacks.func(__VA_ARGS__);

static inline void BeginFrameTime()
{
	FrameCount++;
	if(!FPSFrameCount)
	{
		FPSStart = SDL_GetTicks64();
	}
	BeginTicks = SDL_GetTicks64();
	FPSFrameCount++;
}

static inline void EndFrameTime()
{
	u64 ticks = SDL_GetTicks64();
	Delta = (float)(ticks - BeginTicks) / kDeltaTarget;
	if(ticks - FPSStart >= 1000)
	{
		FPS = FPSFrameCount;
		FPSFrameCount = 0;
	}
}

static inline void UpdateFixedDelta()
{
	// A hard refresh only makes sense if vsync is on.
	if(CurAppSettings.vSync)
	{
		// Only use a hard refresh if we can succesfuly detect the monitor index.
		int displayIndex = SDL_GetWindowDisplayIndex(CurAppWindow);

		// If the display has changed, it's time to update the data.
		if(displayIndex != CurDisplayIndex)
		{
			// Store the new index.
			CurDisplayIndex = displayIndex;

			// Attempt to grab the monitor display mode data.
			SDL_DisplayMode mode;
			if(SDL_GetCurrentDisplayMode(displayIndex, &mode) == 0)
			{
				int hz = Maxi(kFPSTarget, mode.refresh_rate);
				Log_Brief("True refresh rate: %d", mode.refresh_rate);
				Log_Brief("Clamped refresh rate: %d", hz);
				
				// Update the fixed delta value.
				FixedDelta = kFPSTarget / (float)hz;

				// New fixed delta.
				Log_Brief("FixedDelta: %f", FixedDelta);
			}
		}
	}
}

static inline void InitSDL()
{
	// Log SDL version.
	{
		SDL_version version;
		SDL_GetVersion(&version);
		Log_Brief (
			"Using SDL Version %d.%d.%d",
			version.major,
			version.minor,
			version.patch
		);
	}

	// set our graphics driver.
	if(CurAppSettings.driverHint != NULL)
	{
		SDL_SetHintWithPriority (
			SDL_HINT_RENDER_DRIVER,
			CurAppSettings.driverHint,
			SDL_HINT_OVERRIDE
		);
	}

	// try and init SDL.
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Log_FatalError("%s", SDL_GetError());
		App_Shutdown(-1);
	}

	// Init SDL mixer.
	{
		i32 expectedMixFlags = MIX_INIT_MOD | MIX_INIT_OGG;
		if(Mix_Init (expectedMixFlags) != expectedMixFlags)
		{
			Log_FatalError("%s", SDL_GetError());
		}
	}

	// Open the audio device with standard settings.
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0)
	{
		Log_FatalError("%s", SDL_GetError());
	}

	// Allocate the number of simultaneous sounds we want to be able to play.
	Mix_AllocateChannels(NUM_SIMULTANEOUS_SOUNDS);

	// Set volume to 25% by default.
	Mix_MasterVolume(MIX_MAX_VOLUME/4);

	// setup desired window settings.
	SDL_WindowFlags windowFlags = 0;
	u32 window_w = CurAppSettings.width;
	u32 window_h = CurAppSettings.height;

	// if we are in fullscreen mode, set things up with a bit more finesse.
	if(CurAppSettings.fullscreen)
	{
		windowFlags |= SDL_WINDOW_FULLSCREEN;

		// figure out the closest available display mode to the requested values.
		SDL_DisplayMode target;
		SDL_DisplayMode closest;
		target.w = (int)CurAppSettings.width;
		target.h = (int)CurAppSettings.height;
		target.format = 0;  // don't care
		target.refresh_rate = 0; // don't care
		target.driverdata = NULL; // initialize to NULL
		if(!SDL_GetClosestDisplayMode(0, &target, &closest))
		{
			Log_FatalError("%s", SDL_GetError());
		}
		window_w = (u32)closest.w;
		window_h = (u32)closest.h;
	}

	// create the SDL window.
	CurAppWindow = SDL_CreateWindow (
		CurAppSettings.title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(int)window_w,
		(int)window_h,
		windowFlags
	);

	// if window failed to create, error out.
	if(!CurAppWindow)
	{
		Log_FatalError("%s", SDL_GetError());
	}

	// setup renderer flags.
	SDL_RendererFlags renderFlags = SDL_RENDERER_ACCELERATED;
	if(CurAppSettings.vSync)
	{
		renderFlags |= SDL_RENDERER_PRESENTVSYNC;
	}

	// create the renderer.
	CurAppRenderer = SDL_CreateRenderer(CurAppWindow, -1, renderFlags);

	// if renderer failed to create, error out.
	if(!CurAppRenderer)
	{
		Log_FatalError("%s", SDL_GetError());
	}

	// log the current renderer name.
	{
		SDL_RendererInfo info;
		if(SDL_GetRendererInfo(CurAppRenderer, &info) == 0)
		{
			Log_Brief("Render driver: %s", info.name);
		}
	}

	// set the default bg color (50% gray)
	SDL_SetRenderDrawColor(CurAppRenderer, 128, 128, 128, 255);
}

static inline void HandleSDLEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type) {
			case SDL_WINDOWEVENT:
				UpdateFixedDelta();
				break;
			case SDL_QUIT:
				App_Shutdown(0);
				break;
			case SDL_KEYUP:
				if(CurAppSettings.escapeToQuit && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					App_Shutdown(0);
				}
				else
				{
					FireCallback(OnKeyUp, event.key.keysym.scancode, event.key.keysym.mod);
				}
				break;
			case SDL_KEYDOWN:
				FireCallback(OnKeyDown, event.key.keysym.scancode, event.key.keysym.mod);
				break;
			case SDL_MOUSEBUTTONUP:
				FireCallback(OnMouseUp, event.button);
				break;
			case SDL_MOUSEBUTTONDOWN:
				FireCallback(OnMouseDown, event.button);
				break;
			default:
				FireCallback(OnOtherEvent, event);
				break;
		}
	}
}

SDL_Rect App_GetViewport()
{
	return CurAppViewport;
}

SDL_Renderer* App_GetRenderer()
{
	return CurAppRenderer;
}

SDL_Window* App_GetWindow()
{
	return CurAppWindow;
}

AppSettings App_GetSettings()
{
	return CurAppSettings;
}

double App_GetFrameTimeDelta()
{
	return Delta;
}

double App_GetFixedFrameTimeDelta()
{
	// Return the delta.
	return FixedDelta;
}

double App_GetBestFrameDelta()
{
	if(CurAppSettings.vSync)
	{
		return FixedDelta;
	}
	return Delta;
}

u64 App_GetFPS()
{
	return FPS;
}

u64 App_GetFrameCount()
{
	return FrameCount;
}

void App_SetCallbacks(AppCallbacks *callbacks)
{
	CurAppCallbacks = *callbacks;
}

void App_Shutdown(int err)
{
	FireCallback(OnShutdown);
	if(CurAppWindow != NULL)
	{
		SDL_DestroyWindow(CurAppWindow);
	}
	if(CurAppRenderer != NULL)
	{
		SDL_DestroyRenderer(CurAppRenderer);
	}
	Log_Brief("Last FPS: %llu", FPS);
	Mix_Quit();
	SDL_Quit();
	Log_Brief("Exited with code: %d", err);
	exit(err);
}

void App_Run(AppCallbacks *callbacks, AppSettings *settings)
{
	if(callbacks)
	{
		CurAppCallbacks = *callbacks;
	}
	if(settings)
	{
		CurAppSettings = *settings;
	}
	gFatalErrorCallback = App_Shutdown;
	InitSDL();
	UpdateFixedDelta();
	RNG_Randomize(&gRNG, time(NULL));
	SDL_RenderGetViewport(CurAppRenderer, &CurAppViewport);
	DFont_Load();
	LoadStandardTextures();
	FireCallback(OnInit);
	while(true)
	{
		BeginFrameTime();
		SDL_RenderGetViewport(CurAppRenderer, &CurAppViewport);
		HandleSDLEvents();
		FireCallback(OnUpdate);
		SDL_RenderClear(CurAppRenderer);
		FireCallback(OnDraw);
		SDL_RenderPresent(CurAppRenderer);
		EndFrameTime();
	}
}
