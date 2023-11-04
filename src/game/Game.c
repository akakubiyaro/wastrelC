/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Game.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

#define GAME_EVENT(EVENT_NAME, ...) if(Game.events.EVENT_NAME) Game.events.EVENT_NAME(__VA_ARGS__)

struct Game Game = { 0 };


/*----------------------------------------------------------------------------------------------------------------------

	Cardinal

----------------------------------------------------------------------------------------------------------------------*/

static inline u8 OppositeCardinal(u8 c)
{
	u8 r = CARDINAL_NONE;
	if((c & CARDINAL_NORTH) == CARDINAL_NORTH) r |= CARDINAL_SOUTH;
	if((c & CARDINAL_SOUTH) == CARDINAL_SOUTH) r |= CARDINAL_NORTH;
	if((c & CARDINAL_EAST) == CARDINAL_EAST) r |= CARDINAL_WEST;
	if((c & CARDINAL_WEST) == CARDINAL_WEST) r |= CARDINAL_EAST;
	return r;
}

static inline SDL_Point CardinalOffset(u8 c)
{
	SDL_Point offset = { 0, 0 };
	if((c & CARDINAL_NORTH) == CARDINAL_NORTH) offset.y -= 1;
	if((c & CARDINAL_SOUTH) == CARDINAL_SOUTH) offset.y += 1;
	if((c & CARDINAL_EAST) == CARDINAL_EAST) offset.x += 1;
	if((c & CARDINAL_WEST) == CARDINAL_WEST) offset.x -= 1;
	return offset;
}

static inline SDL_Point OppositeCardinalOffset(u8 c)
{
	return CardinalOffset(OppositeCardinal(c));
}


/*----------------------------------------------------------------------------------------------------------------------

	Grid

----------------------------------------------------------------------------------------------------------------------*/

static inline bool InGridBounds(Grid *grid, i32 x, i32 y)
{
	return x < grid->cols && y < grid->rows;
}

static inline Grid *CreateGrid(u32 cols, u32 rows)
{
	Grid *grid = malloc(sizeof(Grid));
	grid->cols = cols;
	grid->rows = rows;
	grid->cells = malloc(sizeof(GridCell*) * cols);
	for(u32 x = 0; x < cols; x++)
	{
		grid->cells = malloc(sizeof(GridCell) * rows);
	}
	for(u32 x = 0; x < cols; x++)
	{
		for(u32 y = 0; y < rows; y++)
		{
			grid->cells[x][y] = (const GridCell) { 0 };
		}
	}
	return grid;
}

static inline GridCell *GetGridCell(Grid *grid, i32 x, i32 y)
{
	if(InGridBounds(grid, x, y))
	{
		return &grid->cells[x][y];
	}
	return NULL;
}

static inline Entity *FindEntityTypeInCell(Grid *grid, u8 type, i32 x, i32 y, Entity *last)
{
	GridCell *cell = GetGridCell(grid, x, y);
	if(cell)
	{
		Entity *cur = last ? last : cell->firstEntity;
		while(cur)
		{
			if(cur->type == type)
			{
				return cur;
			}
			cur = cur->nextInCell;
		}
	}
	return NULL;
}

static inline void FreeGrid(Grid *grid)
{
	for(u32 x = 0; x < grid->cols; x++)
	{
		free(grid->cells[x]);
	}
	free(grid->cells);
	free(grid);
}


/*----------------------------------------------------------------------------------------------------------------------

	Entity

----------------------------------------------------------------------------------------------------------------------*/

static inline GridCell *GetEntityCell(Entity *ent)
{
	if(ent->isOnGrid)
	{
		return GetGridCell(Game.grid, ent->position.x, ent->position.y);
	}
	return NULL;
}

static inline Entity *CreateEntity(u8 type)
{
	Entity *entity = malloc(sizeof(Entity));
	*entity = (const Entity) { 0 };
	entity->type = type;
	return entity;
}

static inline void SpawnEntity(Entity *entity)
{
	if(!entity->isSpawned)
	{
		if(!Game.lastEntity)
		{
			Game.firstEntity = entity;
			Game.lastEntity = entity;
		}
		else
		{
			Game.lastEntity->nextInGame = entity;
			entity->prevInGame = Game.lastEntity;
			Game.lastEntity = entity;
		}
		entity->isSpawned = true;
		GAME_EVENT(OnSpawnEntity, entity);
	}
}

static inline void RemoveEntity(Entity *entity)
{
	if(entity->isOnGrid)
	{
		GridCell *cell = GetEntityCell(entity);
		Entity *next = entity->nextInCell;
		Entity *prev = entity->prevInCell;
		if(next) next->prevInCell = prev;
		if(prev) prev->nextInCell = next;
		if(cell->firstEntity == entity) cell->firstEntity = next;
		if(cell->lastEntity == entity) cell->lastEntity = prev;
		entity->nextInCell = NULL;
		entity->prevInCell = NULL;
		entity->isOnGrid = false;
		GAME_EVENT(OnRemoveEntity, entity);
	}
}

static inline void PlaceEntity(Entity *entity, i32 x, i32 y)
{
	GridCell *cell = GetGridCell(Game.grid, x, y);
	if(cell)
	{
		RemoveEntity(entity);
		if(!cell->lastEntity)
		{
			cell->firstEntity = entity;
			cell->lastEntity = entity;
		}
		else
		{
			cell->lastEntity->nextInCell = entity;
			entity->prevInCell = cell->lastEntity;
			cell->lastEntity = entity;
		}
		entity->isOnGrid = true;
		GAME_EVENT(OnPlaceEntity, entity);
	}
}

static inline void DespawnEntity(Entity *entity)
{
	if(entity->isSpawned)
	{
		RemoveEntity(entity);
		Entity *next = entity->nextInGame;
		Entity *prev = entity->prevInGame;
		if(next) next->prevInGame = prev;
		if(prev) prev->nextInGame = next;
		if(Game.firstEntity == entity) Game.firstEntity = next;
		if(Game.lastEntity == entity) Game.lastEntity = prev;
		entity->prevInGame = NULL;
		entity->nextInGame = NULL;
		entity->isSpawned = false;
		GAME_EVENT(OnDespawnEntity, entity);
	}
}

static inline void FreeEntity(Entity *entity)
{
	DespawnEntity(entity);
	free(entity);
}

static inline void RemoveAllEntities()
{
	Entity *cur = Game.firstEntity;
	while(cur)
	{
		Entity *next = cur->nextInGame;
		RemoveEntity(cur);
		cur = next;
	}
}

static inline void DespawnAndFreeAllEntities()
{
	Entity *cur = Game.firstEntity;
	while(cur)
	{
		Entity *next = cur->nextInGame;
		FreeEntity(cur);
		cur = next;
	}
}


/*----------------------------------------------------------------------------------------------------------------------

	DunGen

----------------------------------------------------------------------------------------------------------------------*/

static inline void GenTestDungeonMap()
{
	static const u32 cols = 40;
	static const u32 rows = 40;
	Game.grid = CreateGrid(cols, rows);
	for(u32 y = 0; y < rows; y++)
	{
		for(u32 x = 0; x < cols; x++)
		{
			Game.grid->cells[x][y].tileId = TILE_ID_FLOOR;
		}
	}
	SpawnEntity(Game.heroEntity);
	PlaceEntity(Game.heroEntity, 10, 10);
	Entity *downStair = CreateEntity(ENT_TYPE_STAIRWAY);
	downStair->stairway.direction = STAIR_DIR_DOWN;
	PlaceEntity(downStair, 20, 20);
	GAME_EVENT(OnMapReady);
}

static inline void ExitMap()
{
	if(Game.grid)
	{
		GAME_EVENT(OnMapExit);

		// Despawn the hero first, we don't want to free their memory.
		DespawnEntity(Game.heroEntity);

		// Everything else can be removed from the grid and freed.
		DespawnAndFreeAllEntities();
		FreeGrid(Game.grid);
		Game.grid = NULL;
	}
}


/*----------------------------------------------------------------------------------------------------------------------

	Game Turn

----------------------------------------------------------------------------------------------------------------------*/

static inline void MoveMonster(Entity *entity)
{
	// TODO: Move the monster in a random direction...
}

static inline void ProcessTurn()
{
	Entity *cur = Game.firstEntity;
	while(cur)
	{
		// Move monsters if we find them.
		if(cur->type == ENT_TYPE_MONSTER)
		{
			MoveMonster(cur);
		}
	}
}


/*----------------------------------------------------------------------------------------------------------------------

	Game

----------------------------------------------------------------------------------------------------------------------*/

void NewGame()
{
	if(Game.isGameInProgress)
	{
		EndGame();
	}
	Game = (const struct Game) { 0 };
	Game.heroEntity = CreateEntity(ENT_TYPE_HERO);
	GAME_EVENT(OnNewGame);
	GenTestDungeonMap();
}

void HeroSkipTurn()
{
	if(Game.isGameInProgress && Game.heroEntity)
	{
		ProcessTurn();
	}
}

bool HeroMove(u8 c)
{
	if(Game.isGameInProgress && Game.heroEntity && Game.heroEntity->isOnGrid && Game.grid)
	{
		SDL_Point p = CardinalOffset(c);
		p.x += Game.heroEntity->position.x;
		p.y += Game.heroEntity->position.y;
		GridCell *cell = GetGridCell(Game.grid, p.x, p.y);
		if(cell && cell->tileId == TILE_ID_FLOOR)
		{
			Entity *monster = FindEntityTypeInCell(Game.grid, ENT_TYPE_MONSTER, p.x, p.y, NULL);
			if(monster)
			{
				FreeEntity(monster);
				return false;
			}
			PlaceEntity(Game.heroEntity, p.x, p.y);
			ProcessTurn();
			return true;
		}
	}
	return false;
}

bool HeroClimbStairs()
{
	if(Game.isGameInProgress && Game.heroEntity && Game.heroEntity->isOnGrid && Game.grid)
	{
		// Try and find a stair in the heroes current location.
		Entity *stair = FindEntityTypeInCell (
			Game.grid, ENT_TYPE_STAIRWAY, Game.heroEntity->position.x, Game.heroEntity->position.y, NULL
		);

		// We expect only one stair.
		if(stair)
		{
			if(stair->stairway.direction == STAIR_DIR_DOWN)
			{
				ExitMap();
				Game.depth++;
				GenTestDungeonMap();
			}
			
			// We assume this is an up stairway.
			else if(Game.depth > 0)
			{
				ExitMap();
				Game.depth--;
				GenTestDungeonMap();
			}
			
			// We climbed up/down stairs.
			return true;
		}
	}
	return false;
}

void EndGame()
{
	Game.isGameInProgress = false;
	GAME_EVENT(OnEndGame);
}