#include "stdafx.h"
#include "Log.h"
#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <thread>

#include <Windows.h>

using namespace std;

string timeString()
{
	auto now = chrono::system_clock::now();
	auto duration = now.time_since_epoch();

	auto mills = chrono::duration_cast<chrono::milliseconds>(duration).count();
	mills %= 1000;

	auto tt = chrono::system_clock::to_time_t(now);
	std::tm tm;
	localtime_s(&tm, &tt);

	char str[64];
	strftime(str, sizeof(str), "%Y/%m/%d %H:%I:%S", &tm);

	stringstream ss;
	ss << str << "." << setw(3) << setfill('0') << left << mills;

	return ss.str();
}

void log(const char* file, const char* func, int line, const char* level, const char* format, ...)
{
	char buf[1024] = { 0 };
	va_list list;

	stringstream ssFunc, ssLoc;
	ssFunc << setw(32) << left << func;
	ssLoc << ssFunc.str().substr(0, 31) << ", " << setw(4) << setfill('0') << line;

	string time = timeString();

	va_start(list, format);
	vsprintf_s(buf, format, list);
	va_end(list);

	stringstream ss;
	ss << "[" << time << "]";
	ss << "[" << ssLoc.str() << "]";
	ss << "[" << setw(6) << this_thread::get_id() << "]";
	ss << "[" << level << "]";
	ss << buf << endl;

	cerr << ss.str();
	OutputDebugStringA(ss.str().c_str());
}

ScopeLog::ScopeLog(const char* scopeName)
{
	_scopeName = scopeName;
	LogInfo("%s ENT", _scopeName);
}

ScopeLog::~ScopeLog()
{
	LogInfo("%s EXT", _scopeName);
}