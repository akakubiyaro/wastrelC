/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Frame.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct Frame Frame;

struct Frame
{
	SDL_RendererFlip flip;
	SDL_Color color;
	SDL_Rect clip;
	SDL_Rect bounds;
	SDL_Texture *texture;
};

static inline Frame DefaultFrame()
{
	return (struct Frame) {
		.flip = SDL_FLIP_NONE,
		.color = (SDL_Color) { 255, 255, 255, 255 },
		.clip = (SDL_Rect) { 0, 0, 0, 0 },
		.bounds = (SDL_Rect) { 0, 0, 0, 0 },
		.texture = NULL
	};
}

static inline i32 RectRight(SDL_Rect r)
{
	return r.x + r.w;
}

static inline i32 RectBottom(SDL_Rect r)
{
	return r.y + r.h;
}

static inline Frame CropFrame(Frame f, SDL_Rect r)
{
	if(f.bounds.y < r.y)
	{
		i32 h = r.y - f.bounds.y;
		float p = (float)h / (float)f.bounds.h;
		f.bounds.h -= h;
		f.bounds.y += h;
		if(f.bounds.h <= 0)
		{
			f.bounds = (const SDL_Rect) { 0, 0, 0, 0 };
			return f;
		}
		h = (i32)((float)f.clip.h * p);
		f.clip.h -= h;
		f.clip.y += h;
	}
	if(f.bounds.x < r.x)
	{
		i32 w = r.x - f.bounds.x;
		float p = (float)w / (float)f.bounds.w;
		f.bounds.w -= w;
		f.bounds.x += w;
		if(f.bounds.w <= 0)
		{
			f.bounds = (const SDL_Rect) { 0, 0, 0, 0 };
			return f;
		}
		w = (i32)((float)f.clip.w * p);
		f.clip.w -= w;
		f.clip.x += w;
	}
	if(RectBottom(f.bounds) > RectBottom(r))
	{
		i32 h = RectBottom(f.bounds) - RectBottom(r);
		float p = (float)h / (float)f.bounds.h;
		f.bounds.h -= h;
		if(f.bounds.h <= 0)
		{
			f.bounds = (const SDL_Rect) { 0, 0, 0, 0 };
			return f;
		}
		h = (i32)((float)f.clip.h * p);
		f.clip.h -= h;
	}
	if(RectRight(f.bounds) > RectRight(r))
	{
		i32 w = RectRight(f.bounds) - RectRight(r);
		float p = (float)w / (float)f.bounds.w;
		f.bounds.w -= w;
		if(f.bounds.w <= 0)
		{
			f.bounds = (const SDL_Rect) { 0, 0, 0, 0 };
			return f;
		}
		w = (i32)((float)f.clip.w * p);
		f.clip.w -= w;
	}
	return f;
}