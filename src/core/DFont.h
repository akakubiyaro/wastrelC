/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	DFont.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

#define DF_IMG_WIDTH 128
#define DF_IMG_HEIGHT 96

#define DF_CHAR_WIDTH 8
#define DF_CHAR_HEIGHT 16

#define DF_COLS 16
#define DF_ROWS 6

TileSet *DFont_Load();
TileSet *DFont_GetPtr();