#include "logger.h"
#include "assertions.h"

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void report_assertion_failure(const char* expression, const char* message, const char* file, i32 line)
{
	logOutput(LOG_LEVEL_FATAL, "Assertion failure: %s, message '%s', in file %s, line: %d\n", expression, message, file, line);
}

SGS void logOutput(e_logLevel level, const char* message, ...)
{
	const char* levelString[6] = { "[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: " };
	b8 isError = level < LOG_LEVEL_WARN;

	const i32 msgLength = 32000;
	char outMessage[msgLength];
	char outputMessage[msgLength];
	memset(outMessage, 0, sizeof(outMessage));

	va_list argPtr;
	va_start(argPtr, message);
	vsnprintf(outMessage, msgLength, message, argPtr);
	va_end(argPtr);

	sprintf_s(outputMessage, "%s%s\n", levelString[level], outMessage);

	if (isError)
	{
		HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
		static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
		SetConsoleTextAttribute(consoleHandle, levels[level]);
		OutputDebugStringA(outputMessage);
		u64 length = strlen(outputMessage);
		LPDWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), outputMessage, (DWORD)length, number_written, 0);
	}
	else {
		HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		static u8 levels[6] = { 64, 4, 6, 2, 1, 8 };
		SetConsoleTextAttribute(console_handle, levels[level]);
		OutputDebugStringA(outputMessage);
		u64 length = strlen(outputMessage);
		LPDWORD number_written = 0;
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), outputMessage, (DWORD)length, number_written, 0);
	}
}