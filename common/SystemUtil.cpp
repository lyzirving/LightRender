#include <chrono>

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