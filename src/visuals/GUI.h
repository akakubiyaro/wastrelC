/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	GUI.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct Window Window;

enum
{
	WINDOW_FLAG_DEFAULT = 0,
	WINDOW_FLAG_BORDERLESS = 1,
	WINDOW_FLAG_CLEAR = 2
};

struct Window
{
	u8 flags; 
	i32 sortIndex;
	SDL_Point scrolling;
	SDL_Rect bounds;
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

extern struct GUI GUI;