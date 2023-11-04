/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	FrameSet.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct FrameSet FrameSet;

struct FrameSet
{
	u32 num;
	Frame *frames;
};

void FrameSet_Init(FrameSet *frameSet);
void FrameSet_AddFrames(FrameSet *frameSet, Frame *frames, u32 num);
void FrameSet_LoadSpriteSheet(FrameSet *frameSet, const char *img, u32 cols, u32 rows, u32 w, u32 h);
void FrameSet_LoadSpriteSheetTexture(FrameSet *frameSet, SDL_Texture *tex, u32 cols, u32 rows, u32 w, u32 h);
Frame FrameSet_GetFrame(FrameSet *frameSet, u32 frameIndex);
Frame FrameSet_GetFrameWrapped(FrameSet *frameSet, u32 frameIndex);
void FrameSet_Free(FrameSet *frameSet);