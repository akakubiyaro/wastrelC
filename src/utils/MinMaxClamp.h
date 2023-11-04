/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	MinMaxClamp.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

static inline i64 Mini(i64 a, i64 b)
{
	return a < b ? a : b;
}

static inline i64 Maxi(i64 a, i64 b)
{
	return a > b ? a : b;
}

static inline float Minf(float a, float b)
{
	return a < b ? a : b;
}

static inline float Maxf(float a, float b)
{
	return a > b ? a : b;
}

static inline i64 Clampi(i64 v, i64 a, i64 b)
{
	if(v < a) v = a;
	if(v > b) v = b;
	return v;
}

static inline float Clampf(float v, float a, float b)
{
	if(v < a) v = a;
	if(v > b) v = b;
	return v;
}