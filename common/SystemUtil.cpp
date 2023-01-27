#include <chrono>
#include <cstdio>
#include <cstdarg>

#include "SystemUtil.h"

int64_t SystemUtil::curTimeMs()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

int64_t SystemUtil::curTimeMicro()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
}

std::string SystemUtil::format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    const auto len = vsnprintf(nullptr, 0, fmt, args);
    va_end(args);

    std::string fmtStr;
    fmtStr.resize(static_cast<size_t>(len) + 1);
    va_start(args, fmt);
    vsnprintf(&fmtStr.front(), len + 1, fmt, args);
    va_end(args);
    fmtStr.resize(static_cast<size_t>(len));

    return fmtStr;
}