#ifndef LOGGER_H
#define	LOGGER_H

#include <string>

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

#endif // !LOGGER_H

