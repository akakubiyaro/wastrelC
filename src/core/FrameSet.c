/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	FrameSet.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

void FrameSet_Init(FrameSet *frameSet)
{
	frameSet->num = 0;
	frameSet->frames = NULL;
}

void FrameSet_AddFrames(FrameSet *frameSet, Frame *frames, u32 num)
{
	u32 i = frameSet->num;
	frameSet->num += num;
	frameSet->frames = realloc(frameSet->frames, sizeof(Frame) * frameSet->num);
	for(u32 j = 0; i < frameSet->num; i++, j++)
	{
		frameSet->frames[i] = frames[j];
	}
}

void FrameSet_LoadSpriteSheet(FrameSet *frameSet, const char *img, u32 cols, u32 rows, u32 w, u32 h)
{
	SDL_Texture *texture = LoadTextureFromImageFile(img);
	if(texture)
	{
		FrameSet_LoadSpriteSheetTexture(frameSet, texture, cols, rows, w, h);
	}
	else
	{
		Log_Warning("Could not load sprite sheet: '%s'", img);
	}
}

void FrameSet_LoadSpriteSheetTexture(FrameSet *frameSet, SDL_Texture *tex, u32 cols, u32 rows, u32 w, u32 h)
{
	int texW, texH;
	SDL_QueryTexture(tex, NULL, NULL, &texW, &texH);
	int clipW = texW / cols;
	int clipH = texH / rows;
	for(u32 y = 0; y < rows; y++)
	{
		for(u32 x = 0; x < cols; x++)
		{
			Frame frame = DefaultFrame();
			frame.texture = tex;
			frame.clip = (SDL_Rect) { x * clipW, y * clipH, clipW, clipH };
			frame.bounds = (SDL_Rect) { 0, 0, w, h };
			FrameSet_AddFrames(frameSet, &frame, 1);
		}
	}
}

Frame FrameSet_GetFrame(FrameSet *frameSet, u32 frameIndex)
{
	if(frameIndex < frameSet->num)
	{
		return frameSet->frames[frameIndex];
	}
	return DefaultFrame();
}

Frame FrameSet_GetFrameWrapped(FrameSet *frameSet, u32 frameIndex)
{
	if(frameSet->num)
	{
		return frameSet->frames[frameIndex % frameSet->num];
	}
	return DefaultFrame();
}

void FrameSet_Free(FrameSet *frameSet)
{
	if(frameSet->frames)
	{
		free(frameSet->frames);
	}
	FrameSet_Init(frameSet);
}
