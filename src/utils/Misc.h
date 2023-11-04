/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Misc.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

static inline u32 XYToIndex(u32 x, u32 y, u32 cols)
{
	return (y * cols) + x;
}

static inline void IndexToXY(u32 i, u32 *x, u32 *y, u32 cols)
{
	if(x) *x = i % cols;
	if(y) *y = i / cols;
}