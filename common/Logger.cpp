#include <glog/logging.h>
#pragma comment(lib, "glog.lib")
#include <Windows.h>

#include <io.h>
#include <direct.h>
#include <cstdio>

#include "Logger.h"

void Logger::init()
{
    google::InitGoogleLogging("LightRender");
    FLAGS_alsologtostderr = true;
    FLAGS_colorlogtostderr = true;

#ifndef TYPE_DEBUG

    std::string root = getRootPath();
    root.append("/logs");

    if (_access(root.c_str(), 0) == -1) //root directory does not exist
    {
        int res = _mkdir(root.c_str());
    }

    std::string infoPath = root + "/INFO_";
    std::string warnPath = root + "/WARN_";
    std::string errPath = root + "/ERR_";

    google::SetLogDestination(google::GLOG_INFO, infoPath.c_str());
    google::SetLogDestination(google::GLOG_WARNING, warnPath.c_str());
    google::SetLogDestination(google::GLOG_ERROR, errPath.c_str());

#endif // !TYPE_DEBUG

}

void Logger::release()
{
    google::ShutdownGoogleLogging();
}

void Logger::info(const char* fmt, ...)
{
    char buf[512] = {0};

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);

    LOG(INFO) << buf;
}

void Logger::warn(const char* fmt, ...)
{
    char buf[512] = { 0 };

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);

    LOG(WARNING) << buf;
}

void Logger::err(const char* fmt, ...)
{
    char buf[512] = { 0 };

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);

    LOG(ERROR) << buf;
}

std::string Logger::getRootPath()
{
    char path[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, path);

    std::string root = path;
    for (auto itr = root.begin(); itr != root.end(); itr++)
    {
        if (*itr == '\\')
            *itr = '/';
    }
    return root;
}
