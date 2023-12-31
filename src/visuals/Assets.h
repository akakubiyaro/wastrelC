/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Assets.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

#define TILE_W 32
#define TILE_H 32

#define GLYPH_W 16
#define GLYPH_H 16

struct Assets
{
	TileSet figureTiles;
	TileSet mapTiles;
	TileSet glyphTiles;
	TileSet guiTiles;
};

extern struct Assets Assets;
