#ifndef LOGGER_H
#define	LOGGER_H

#include <string>

#ifdef LIG_TAG
#undef LIG_TAG
#endif
#define LIG_TAG "LightRender"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Logger"

class Logger
{
public:

	static void init();
	static void release();

	static void info(const char* fmt, ...);
	static void warn(const char* fmt, ...);
	static void err(const char* fmt, ...);
private:
	Logger() {}
	virtual ~Logger() = default;

	static std::string getRootPath();
};

#define LOG_INFO(fmt, ...)									   \
	        {												   \
				Logger::info("[%s]:[%s][%s][%d] " fmt,		   \
							 LIG_TAG, LOCAL_TAG, __FUNCTION__, \
							 __LINE__, ##__VA_ARGS__);		   \
			}

#define LOG_WARN(fmt, ...)									   \
	        {												   \
				Logger::warn("[%s]:[%s][%s][%d] " fmt,		   \
							 LIG_TAG, LOCAL_TAG, __FUNCTION__, \
							 __LINE__, ##__VA_ARGS__);		   \
			}

#define LOG_ERR(fmt, ...)									   \
	        {												   \
				Logger::err("[%s]:[%s][%s][%d] " fmt,	       \
							 LIG_TAG, LOCAL_TAG, __FUNCTION__, \
							 __LINE__, ##__VA_ARGS__);		   \
			}

#endif // !LOGGER_H

