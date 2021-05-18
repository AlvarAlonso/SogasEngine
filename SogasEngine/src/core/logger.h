#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

typedef enum e_logLevel {
	LOG_LEVEL_FATAL = 0,
	LOG_LEVEL_ERROR = 1,
	LOG_LEVEL_WARN = 2,
	LOG_LEVEL_INFO = 3,
	LOG_LEVEL_DEBUG = 4,
	LOG_LEVEL_TRACE = 5
}e_logLevel;

SGS void logOutput(e_logLevel level, const char* message, ...);

#define SGSFATAL(message, ...) logOutput(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);

#ifndef SGSERROR
#define SGSERROR(message, ...) logOutput(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

#if LOG_WARN_ENABLED == 1
#define SGSWARN(message, ...) logOutput(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
#define SGSWARN(message, ...);
#endif

#if LOG_INFO_ENABLED == 1
#define SGSINFO(message, ...) logOutput(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
#define SGSINFO(message, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
#define SGSDEBUG(message, ...) logOutput(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
#define SGSDEBUG(message, ...)
#endif

#if LOG_TRACE_ENABLED == 1
#define SGSTRACE(message, ...) logOutput(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
#define SGSTRACE(message, ...)
#endif