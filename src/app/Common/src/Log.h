#pragma once


#if defined(_DEBUG)
#define LogDebug(fmt, ...)	log(__FILE__, __FUNCTION__, __LINE__, "DEBUG", fmt, __VA_ARGS__)
#define LogInfo(fmt, ...)	log(__FILE__, __FUNCTION__, __LINE__, "INFO ", fmt, __VA_ARGS__)
#define LogWarn(fmt, ...)	log(__FILE__, __FUNCTION__, __LINE__, "WARN ", fmt, __VA_ARGS__)
#define LogError(fmt, ...)	log(__FILE__, __FUNCTION__, __LINE__, "ERROR", fmt, __VA_ARGS__)
#define LogFatal(fmt, ...)	log(__FILE__, __FUNCTION__, __LINE__, "FATAL", fmt, __VA_ARGS__)
#define LogEntExt			ScopeLog scopeLog__(__FUNCTION__)
#else
#define LogDebug(fmt, ...)
#define LogInfo(fmt, ...)
#define LogWarn(...)
#define LogWarn(fmt, ...)
#define LogError(fmt, ...)
#define LogFatal(fmt, ...)
#endif

void log(const char* file, const char* func, int line, const char* level, const char* format, ...);

class ScopeLog {
public:
	ScopeLog(const char* scopeName);
	~ScopeLog();
private:
	const char* _scopeName;
};