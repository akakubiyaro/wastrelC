/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	RNG.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

RNG gRNG = { .seed = 0, .s = { 1234567890, 9876543210 } };

// Based on: https://prng.di.unimi.it/xoroshiro128plus.c , http://xorshift.di.unimi.it/splitmix64.c

static inline u64 RotL(const u64 x, int k)
{
	return (x << k) | (x >> (64 - k));
}

void RNG_Randomize(RNG *state, u64 seed)
{
	u64 z;

	// Store initial seed.
	state->seed = seed;

	// Calc first seed.
	z = (seed += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	state->s[0] = z ^ (z >> 31);

	// Calc second seed.
	z = (state->s[0] += 0x9e3779b97f4a7c15);
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	state->s[1] = z ^ (z >> 31);
}

u64 RNG_Value(RNG *state)
{
	const u64 s0 = state->s[0];
	u64 s1 = state->s[1];
	const u64 result = s0 + s1;
	s1 ^= s0;
	state->s[0] = RotL(s0, 55) ^ s1 ^ (s1 << 14); // a, b
	state->s[1] = RotL(s1, 36); // c
	return result;
}

u64 RNG_Range(RNG *state, u64 min, u64 max)
{
	if(min > max)
	{
		u64 tmp = min;
		min = max;
		max = tmp;
	}
	if(min == max) return min;
	return RNG_Value(state) % ((max + 1) - min) + min;
}

bool RNG_Chance(RNG *state, u64 percent)
{
	return RNG_Range(state, 1, 100) <= percent;
}

bool RNG_ChanceOutOf(RNG *state, u64 num, u64 outOf)
{
	return RNG_Range(state, 1, outOf) <= num;
}