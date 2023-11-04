/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	TileSet.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct TileSet TileSet;

struct TileSet
{
	u32 num;
	Tile *tiles;
};

void TileSet_Init(TileSet *tileSet);
Tile *TileSet_GetTile(TileSet *tileSet, u32 tileId);
Tile *TileSet_GetTileWrapped(TileSet *tileSet, u32 tileId);
Frame TileSet_GetTileFrame(TileSet *tileSet, u32 tileId);
Frame TileSet_GetTileFrameWrapped(TileSet *tileSet, u32 tileId);
void TileSet_ResetOffsets(TileSet *tileSet);
void TileSet_AddFrameSet(TileSet *tileSet, FrameSet *frameSet, u32 range);
void TileSet_LoadSpriteSheet(TileSet *tileSet, const char *imgPath, u32 cols, u32 rows, u32 w, u32 h, u32 range);
void TileSet_LoadSpriteSheetTexture(TileSet *tileSet, SDL_Texture *tex, u32 cols, u32 rows, u32 w, u32 h, u32 range);
void TileSet_Free(TileSet *tileSet);
