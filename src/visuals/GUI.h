/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	GUI.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct Glyph Glyph;
typedef struct TextArea TextArea;
typedef struct Window Window;

enum
{
	WINDOW_FLAG_DEFAULT = 0,
	WINDOW_FLAG_BORDERLESS = 1,
	WINDOW_FLAG_CLEAR = 2
};

struct Glyph
{
	u8 fg, bg;
	i16 c;
};

struct TextArea
{
	u32 cols, rows;
	Glyph **glyphs;
};

struct Window
{
	u8 flags; 
	i32 sortIndex;
	SDL_Rect bounds;
	SDL_Rect content;
	TextArea textArea;
};

struct GUI
{
	struct 
	{
		bool needsSort;
		u32 num, cap;
		Window **windows;

	} windowList;
};

extern GUI GUI;