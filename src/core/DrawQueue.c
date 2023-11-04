/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	DrawQueue.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

static int CmpDrawRequests(const void *a, const void *b)
{
	DrawRequest *reqA = ((DrawRequest*)a);
	DrawRequest *reqB = ((DrawRequest*)b);
	if(reqA->sortIndex == reqB->sortIndex)
	{
		return reqA->addIndex - reqB->addIndex;
	}
	return reqA->sortIndex - reqB->sortIndex;
}

void DrawQueue_Init(DrawQueue *dq, u32 cap)
{
	dq->num = 0;
	dq->cap = cap;
	dq->requests = NULL;
	if(dq->cap)
	{
		dq->requests = malloc(sizeof(DrawRequest) * dq->cap);
	}
}

void DrawQueue_Clear(DrawQueue *dq)
{
	dq->num = 0;
}

void DrawQueue_AddRequest(DrawQueue *dq, DrawRequest *req)
{
	// Reject frames with negative or zero area.
	if(req->frame.bounds.w * req->frame.bounds.h <= 0)
	{
		return;
	}

	// Check the request against the viewport, reject it if it's not in bounds.
	SDL_Rect viewport = App_GetViewport();
	if(SDL_HasIntersection(&viewport, &req->frame.bounds) == 1)
	{
		if(dq->num >= dq->cap)
		{
			dq->cap = dq->cap ? dq->cap * 2 : 2;
			dq->requests = realloc(dq->requests, sizeof(DrawRequest) * dq->cap);
		}
		req->addIndex = dq->num++;
		dq->requests[req->addIndex] = *req;
	}
}

void DrawQueue_AddFrame(DrawQueue *dq, Frame *frame, i32 sortIndex)
{
	DrawRequest req;
	req.sortIndex = sortIndex;
	req.frame = *frame;
	DrawQueue_AddRequest(dq, &req);
}

void DrawQueue_Draw(DrawQueue *dq)
{
	for(u32 i = 0; i < dq->num; i++)
	{
		// Grab the draw request.
		DrawRequest *req = &dq->requests[i];

		// Don't even bother if the alpha is zero.
		if(req->frame.color.a == 0)
		{
			return;
		}

		// Setup color modifications.
		SDL_SetTextureColorMod(req->frame.texture, req->frame.color.r, req->frame.color.g, req->frame.color.b);
		SDL_SetTextureAlphaMod(req->frame.texture, req->frame.color.a);

		// Draw our quad.
		SDL_RenderCopyEx (
			App_GetRenderer(),
			req->frame.texture,
			&req->frame.clip,
			&req->frame.bounds,
			0.0f,
			NULL,
			req->frame.flip
		);
	}
}

void DrawQueue_Sort(DrawQueue *dq)
{
	qsort(dq->requests, dq->num, sizeof(DrawRequest), CmpDrawRequests);
}

void DrawQueue_SortRange(DrawQueue *dq, u32 start, u32 end)
{
	qsort(dq->requests + start, end - start, sizeof(DrawRequest), CmpDrawRequests);
}

void DrawQueue_Free(DrawQueue *dq)
{
	if(dq->requests)
	{
		free(dq->requests);
	}
	DrawQueue_Init(dq, 0);
}

