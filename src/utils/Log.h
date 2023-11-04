/*----------------------------------------------------------------------------------------------------------------------
========================================================================================================================

	Log.h

	Copyright 2023 by Michael Cabral.

========================================================================================================================
----------------------------------------------------------------------------------------------------------------------*/

#pragma once

#include <Wastrel.h>

#define kMaxLogMsgLen 1024

#define Log_Brief(MSG, ...) fprintf(stdout, "[INFO]: " MSG "\n", ##__VA_ARGS__)
#define Log_BriefErr(MSG, ...) fprintf(stderr, "[ERROR]: " MSG "\n", ##__VA_ARGS__)
#define Log_Info(MSG, ...) Log_Post(stdout, "INFO", MSG, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define Log_Debug(MSG, ...) Log_Post(stdout, "DEBUG", MSG, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define Log_Warning(MSG, ...) Log_Post(stdout, "WARNING", MSG, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define Log_Error(MSG, ...) Log_Post(stderr, "ERROR", MSG, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define Log_Assert(CMD) { if(!(CMD)) { Log_FatalError("Assert '" #CMD "' failed!"); } }
#define Log_NullCheck(PTR) if(!PTR) Log_FatalError("Null check failed!")
#define Log_MemCheck(PTR) if(!PTR) Log_FatalError("Memory allocation failed!")
#define Log_Unreachable() Log_FatalError("Unreachable code reached!")
#define Log_Unimplemented() Log_FatalError("Unimplemented code reached!")
#define Log_FatalError(MSG, ...) { Log_Post(stderr, "FATAL", MSG, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
	if(gFatalErrorCallback) gFatalErrorCallback(-1); }

extern void (*gFatalErrorCallback)(int errCode);

const char *Log_GetLastMSG();
void Log_Post(FILE *stream, const char *prefix, const char *msg, const char *file, const char *func, u32 line, ...);
