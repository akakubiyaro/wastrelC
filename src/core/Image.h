/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Image.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

extern SDL_Texture *gWhiteTexture;
extern SDL_Texture *gBlackTexture;

void LoadStandardTextures();
SDL_Surface *LoadSurfaceFromImageFile(const char *path);
SDL_Surface *LoadSurfaceFromColor(u32 w, u32 h, u8 r, u8 g, u8 b, u8 a);
void SetImagePixel(SDL_Surface *surface, u32 x, u32 y, u8 r, u8 g, u8 b, u8 a);
SDL_Texture *LoadTextureFromImageFile(const char *path);
SDL_Texture *LoadTextureFromSurface(SDL_Surface *surface);
SDL_Texture *LoadTextureFromColor(u32 w, u32 h, u8 r, u8 g, u8 b, u8 a);