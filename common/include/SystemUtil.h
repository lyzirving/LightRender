#ifndef SYSTEM_UTIL_H
#define SYSTEM_UTIL_H

#include <stdint.h>
#include <string>

class SystemUtil
{
public:

	static int64_t curTimeMs();
	static int64_t curTimeMicro();
	static std::string format(const char* fmt, ...);

private:
	SystemUtil() {}
	~SystemUtil() = default;
};

#endif // !SYSTEM_UTIL_H

