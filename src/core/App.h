/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	App.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

// We shoot for 60FPS
#define kFPSTarget 60.0

typedef struct AppSettings AppSettings;
typedef struct AppCallbacks AppCallbacks;

struct AppSettings
{
	const char *title;
	const char *driverHint;
	u32 width;
	u32 height;
	bool fullscreen;
	bool vSync;
	bool escapeToQuit;
};

struct AppCallbacks
{
	void (*OnInit)();
	void (*OnKeyUp)(SDL_Scancode code, SDL_Keymod mod);
	void (*OnKeyDown)(SDL_Scancode code, SDL_Keymod mod);
	void (*OnMouseUp)(SDL_MouseButtonEvent button);
	void (*OnMouseDown)(SDL_MouseButtonEvent button);
	void (*OnOtherEvent)(SDL_Event event);
	void (*OnUpdate)();
	void (*OnDraw)();
	void (*OnShutdown)();
};

SDL_Rect App_GetViewport();
SDL_Renderer* App_GetRenderer();
SDL_Window* App_GetWindow();
AppSettings App_GetSettings();
double App_GetFrameTimeDelta();
double App_GetFixedFrameTimeDelta();
double App_GetBestFrameDelta();
u64 App_GetFPS();
u64 App_GetFrameCount();
void App_SetCallbacks(AppCallbacks *callbacks);
void App_Shutdown(int err);
void App_Run(AppCallbacks *callbacks, AppSettings *settings);