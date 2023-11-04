/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	DrawQueue.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

typedef struct DrawRequest DrawRequest;
typedef struct DrawQueue DrawQueue;

struct DrawRequest
{
	i32 addIndex, sortIndex;
	Frame frame;
};

struct DrawQueue
{
	u32 num, cap;
	DrawRequest *requests;
};

static inline DrawRequest DefaultDrawRequest()
{
	return (DrawRequest) {
		.addIndex = 0,
		.sortIndex = 0,
		.frame = DefaultFrame()
	};
}

void DrawQueue_Init(DrawQueue *dq, u32 cap);
void DrawQueue_Clear(DrawQueue *dq);
void DrawQueue_AddRequest(DrawQueue *dq, DrawRequest *req);
void DrawQueue_AddFrame(DrawQueue *dq, Frame *frame, i32 sortIndex);
void DrawQueue_Draw(DrawQueue *dq);
void DrawQueue_Sort(DrawQueue *dq);
void DrawQueue_SortRange(DrawQueue *dq, u32 start, u32 end);
void DrawQueue_Free(DrawQueue *dq);
