/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	RNG.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct RNG RNG;

struct RNG
{
	u64 seed;
	u64 s[2];
};

extern struct RNG gRNG;

void RNG_Randomize(RNG *state, u64 seed);
u64 RNG_Value(RNG *state);
u64 RNG_Range(RNG *state, u64 min, u64 max);
bool RNG_Chance(RNG *state, u64 percent);
bool RNG_ChanceOutOf(RNG *state, u64 num, u64 outOf);