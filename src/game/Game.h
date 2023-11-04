/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Game.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

typedef struct Entity Entity;
typedef struct HeroEntity HeroEntity;
typedef struct MonsterEntity MonsterEntity;
typedef struct DoorwayEntity DoorwayEntity;
typedef struct StairwayEntity StairwayEntity;
typedef struct GridCell GridCell;
typedef struct Grid Grid;
typedef struct GameEvents GameEvents;

enum
{
    CARDINAL_NONE = 0,
    CARDINAL_NORTH = 1,
    CARDINAL_SOUTH = 2,
    CARDINAL_EAST = 4,
    CARDINAL_WEST = 8,
    CARDINAL_NORTH_EAST = CARDINAL_NORTH | CARDINAL_EAST,
    CARDINAL_SOUTH_EAST = CARDINAL_SOUTH | CARDINAL_EAST,
    CARDINAL_NORTH_WEST = CARDINAL_NORTH | CARDINAL_WEST,
    CARDINAL_NORTH_ESAT = CARDINAL_NORTH | CARDINAL_EAST
};

enum
{
    TILE_ID_NONE = 0,
    TILE_ID_FLOOR,
    TILE_ID_WALL,
    TILE_ID_WATER
};

enum
{
    ENT_TYPE_HERO,
    ENT_TYPE_MONSTER,
    ENT_TYPE_DOORWAY,
    ENT_TYPE_STAIRWAY
};

enum
{
    STAIR_DIR_DOWN,
    STAIR_DIR_UP
};

struct HeroEntity
{
    // TODO: ...
};

struct MonsterEntity
{
    // TODO: ...
};

struct DoorwayEntity
{
    u8 direction;
    u8 state;
};

struct StairwayEntity
{
    u8 direction;
};

struct Entity
{
    u8 type;

    bool isSpawned;
    bool isOnGrid;

    SDL_Point position;

    Entity *nextInGame;
    Entity *prevInGame;
    Entity *nextInCell;
    Entity *prevInCell;

    union
    {
        HeroEntity hero;
        MonsterEntity monster;
        DoorwayEntity doorway;
        StairwayEntity stairway;
    };

    void *userData;
};

struct GridCell
{
    u8 tileId;

    Entity *firstEntity, *lastEntity;
};

struct Grid
{
    u32 cols;
    u32 rows;

    GridCell **cells;
};

struct GameEvents
{
    void (*OnNewGame)();
    void (*OnMapReady)();
    void (*OnMapExit)();
    void (*OnSpawnEntity)(Entity *entity);
    void (*OnPlaceEntity)(Entity *entity);
    void (*OnRemoveEntity)(Entity *entity);
    void (*OnDespawnEntity)(Entity *entity);
    void (*OnEndGame)();
};

struct Game
{
    bool isGameInProgress;

    u64 turnCount;

    u32 depth;

    Grid *grid;

    Entity *heroEntity;
    Entity *firstEntity;
    Entity *lastEntity;

    GameEvents events;
};

extern struct Game Game;

void NewGame();
void HeroSkipTurn();

bool HeroMove(u8 c);
bool HeroClimbStairs();

void EndGame();