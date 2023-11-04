/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Image.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

SDL_Texture *gWhiteTexture = NULL;
SDL_Texture *gBlackTexture = NULL;

void LoadStandardTextures()
{
	gWhiteTexture = LoadTextureFromColor(1, 1, 255, 255, 255, 255);
	gBlackTexture = LoadTextureFromColor(1, 1, 0, 0, 0, 255);
}

SDL_Surface *LoadSurfaceFromImageFile(const char *path)
{
	// Load the image.
	SDL_Surface *surf = IMG_Load(path);
	if(surf == NULL)
	{
		Log_Error("Could not load image %s: %s", path, SDL_GetError());
		return NULL;
	}

	// Let us know if it worked.
	Log_Brief("Loaded image: '%s'", path);

	// Return the final surface.
	return surf;
}

SDL_Surface *LoadSurfaceFromColor(u32 w, u32 h, u8 r, u8 g, u8 b, u8 a)
{
	u32 rmask = 0, gmask = 0, bmask = 0, amask = 0;
	if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
	{
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	}
	else
	{
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	}
	SDL_Surface *s = SDL_CreateRGBSurface(0, w, h, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(s, NULL, SDL_MapRGBA(s->format, r, g, b, a));
	return s;
}

void SetImagePixel(SDL_Surface *surface, u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
{
	u32 *pixels = (u32*)surface->pixels;
	pixels[ ( y * surface->w ) + x ] = SDL_MapRGBA(surface->format, r, g, b, a);
}

SDL_Texture *LoadTextureFromImageFile(const char *path)
{
	SDL_Surface *surface = LoadSurfaceFromImageFile(path);
	if(surface)
	{
		SDL_Texture *texture = LoadTextureFromSurface(surface);
		SDL_FreeSurface(surface);
		if(texture)
		{
			Log_Brief("Texture created from: '%s'", path);
			return texture;
		}
	}
	return NULL;
}

SDL_Texture *LoadTextureFromSurface(SDL_Surface *surface)
{
	SDL_Texture *tex = SDL_CreateTextureFromSurface(App_GetRenderer(), surface);
	if(!tex)
	{
		Log_Error("Error creating texture: '%s'", SDL_GetError());
	}
	return tex;
}

SDL_Texture *LoadTextureFromColor(u32 w, u32 h, u8 r, u8 g, u8 b, u8 a)
{
	SDL_Surface *surface = LoadSurfaceFromColor(w, h, r, g, b, a);
	if(surface)
	{
		SDL_Texture *texture = LoadTextureFromSurface(surface);
		SDL_FreeSurface(surface);
		if(texture)
		{
			return texture;
		}
	}
	return NULL;
}