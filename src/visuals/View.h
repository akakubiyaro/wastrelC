/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	View.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct Actor Actor;
typedef struct Stage Stage;

struct Actor
{
	u16 tileId;
	i32 sortIndex;
	SDL_Point position;
};

struct Stage
{
	u32 cols, rows;
	i16 **tileIds;
};

struct View
{
	SDL_Point position;
	u32 numActore;
	Stage *stage;
	Actor **actors;
};

extern struct View View;

void UpdateView();
void DrawView();