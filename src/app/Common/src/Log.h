#pragma once


#if defined(_DEBUG)
#define LogInfo(fmt, ...) log(__FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#define LogWarn(fmt, ...) log(__FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__)
#else
#define LogWarn(...)
#endif

void log(char* file, char* func, int line, const char* format, ...);
