/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	TileSet.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

void TileSet_Init(TileSet *tileSet)
{
	tileSet->num = 0;
	tileSet->tiles = NULL;
}

Tile *TileSet_GetTile(TileSet *tileSet, u32 tileId)
{
	if(tileId < tileSet->num)
	{
		return &tileSet->tiles[tileId];
	}
	return NULL;
}

Tile *TileSet_GetTileWrapped(TileSet *tileSet, u32 tileId)
{
	if(tileSet->num)
	{
		return &tileSet->tiles[tileId % tileSet->num];
	}
	return NULL;
}

void TileSet_ResetOffsets(TileSet *tileSet)
{
	for(u32 i = 0; i < tileSet->num; i++)
	{
		tileSet->tiles[i].offset = 0;
	}
}

Frame TileSet_GetTileFrame(TileSet *tileSet, u32 tileId)
{
	Tile *tile = TileSet_GetTile(tileSet, tileId);
	if(tile)
	{
		return FrameSet_GetFrameWrapped(tile->frameSet, tile->frameIndex + tile->offset);
	}
	return DefaultFrame();
}

Frame TileSet_GetTileFrameWrapped(TileSet *tileSet, u32 tileId)
{
	Tile *tile = TileSet_GetTileWrapped(tileSet, tileId);
	if(tile)
	{
		return FrameSet_GetFrameWrapped(tile->frameSet, tile->frameIndex + tile->offset);
	}
	return DefaultFrame();
}

void TileSet_AddFrameSet(TileSet *tileSet, FrameSet *frameSet, u32 range)
{
	u32 numToAdd = frameSet->num / range;
	if(!numToAdd)
	{
		return;
	}
	u32 i = tileSet->num;
	tileSet->num += numToAdd;
	tileSet->tiles = realloc(tileSet->tiles, sizeof(Tile) * tileSet->num);
	for(u32 j = 0; j < numToAdd; j++, i++)
	{
		tileSet->tiles[i] = (Tile) {
			.frameIndex = j * range,
			.offset = 0,
			.frameSet = frameSet
		};
	}
}

void TileSet_LoadSpriteSheet(TileSet *tileSet, const char *imgPath, u32 cols, u32 rows, u32 w, u32 h, u32 range)
{
	range = Maxi(range, 1);
	FrameSet *frameSet = malloc(sizeof(FrameSet));
	FrameSet_Init(frameSet);
	FrameSet_LoadSpriteSheet(frameSet, imgPath, cols, rows, w, h);
	if(frameSet->num)
	{
		TileSet_AddFrameSet(tileSet, frameSet, range);
	}
	else
	{
		free(frameSet);
	}
}

void TileSet_LoadSpriteSheetTexture(TileSet *tileSet, SDL_Texture *tex, u32 cols, u32 rows, u32 w, u32 h, u32 range)
{
	range = Maxi(range, 1);
	FrameSet *frameSet = malloc(sizeof(FrameSet));
	FrameSet_Init(frameSet);
	FrameSet_LoadSpriteSheetTexture(frameSet, tex, cols, rows, w, h);
	if(frameSet->num)
	{
		TileSet_AddFrameSet(tileSet, frameSet, range);
	}
	else
	{
		free(frameSet);
	}
}

void TileSet_Free(TileSet *tileSet)
{
	if(tileSet->tiles)
	{
		free(tileSet->tiles);
	}
	TileSet_Init(tileSet);
}

