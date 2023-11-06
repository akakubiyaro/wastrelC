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
	bool isInView;
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
	bool needsActorSort;
	SDL_Point position;
	u32 numActore;
	Stage *stage;
	Actor **actors;
};

extern struct View View;

Actor *CreateActor();
void AddActorToView(Actor *actor);
void SetActorSortIndex(Actor *actor, i32 sortIndex);
void ChangeActorSortIndex(Actor *actor, i32 sortIndex);
void RemoveActorFromView(Actor *actor);
void FreeActor(Actor *actor);
void UpdateView();
void DrawView();