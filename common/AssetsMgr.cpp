#include <mutex>
#include <cassert>
#include <Windows.h>
#include <direct.h>
#include <algorithm>

#include "AssetsMgr.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AssetsMgr"

static std::mutex g_mutex{};
static AssetsMgr* g_mgr{ nullptr };

AssetsMgr* AssetsMgr::get()
{
    if (!g_mgr) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_mgr)
            g_mgr = new AssetsMgr;
    }
    return g_mgr;
}

AssetsMgr::AssetsMgr() = default;

AssetsMgr::~AssetsMgr() = default;

std::string AssetsMgr::getPath(const std::string& path, SRC_TYPE type)
{
    char *buf = getcwd(nullptr, 0);
    if (!buf)
    {
        LOG_ERR("getcwd failed");
        assert(0);
    }
    std::string root = std::string(buf);
    std::free(buf);
    const char oldVal = '\\';
    const char newVal = '/';
    std::replace(root.begin(), root.end(), oldVal, newVal);

#ifdef _WIN32
    root.append("/x64");

#ifdef TYPE_DEBUG
    root.append("/Debug");
#else
    root.append("/Release");
#endif // TYPE_DEBUG

#else 
    LOG_ERR("unknown platform");
    assert(0);
#endif // _WIN32
    switch (type)
    {
    case SHADER:
    {
        return root + std::string("/") + "shader" + "/" + path;
    }
    case TEXTURE:
    {
        return root + std::string("/") + "texture" + "/" + path;
    }
    case MODEL:
    default:
    {
        return root + std::string("/") + "model" + "/" + path;
    }
    }
}

std::string AssetsMgr::getObj(const std::string& name)
{
    std::string path(name);
    path.append("/");
    path.append(name);
    path.append(".obj");
    return getPath(path, SRC_TYPE::MODEL);
}

std::string AssetsMgr::getTexture(const std::string& name, PIC_TYPE type)
{
    std::string path(name);
    if (type == PIC_TYPE::JPG)
        path.append(".jpg");
    else
        path.append(".png");
    return getPath(path, SRC_TYPE::TEXTURE);
}

std::string AssetsMgr::getVert(const std::string& name)
{
    std::string path(name);
    path.append(".vert");
    return getPath(path, SRC_TYPE::SHADER);
}

std::string AssetsMgr::getFrag(const std::string& name)
{
    std::string path(name);
    path.append(".frag");
    return getPath(path, SRC_TYPE::SHADER);
}

