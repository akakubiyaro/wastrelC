/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Log.c

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#include <Wastrel.h>

void (*gFatalErrorCallback)(int errCode) = NULL;

static char LastMSG[kMaxLogMsgLen] = "\0";

const char *Log_GetLastMSG()
{
	return LastMSG;
}

void Log_Post(FILE *stream, const char *prefix, const char *msg, const char *file, const char *func, u32 line, ...)
{
	// Some vargs manipulation.
	va_list args;
	va_start(args, line);
	vsnprintf(LastMSG, kMaxLogMsgLen, msg, args);
	va_end(args);

	// Print it out.
	fprintf(stream, "[%s]: %s\n --> %s:%s():%u\n", prefix, LastMSG, file, func, line);
}
