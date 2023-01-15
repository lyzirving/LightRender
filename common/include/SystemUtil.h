#ifndef SYSTEM_UTIL_H
#define SYSTEM_UTIL_H

#include <stdint.h>

class SystemUtil
{
public:

	static int64_t curTimeMs();
	static int64_t curTimeMicro();

private:
	SystemUtil() {}
	~SystemUtil() = default;
};

#endif // !SYSTEM_UTIL_H

