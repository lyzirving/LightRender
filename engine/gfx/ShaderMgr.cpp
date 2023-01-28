#include <mutex>

#include "ShaderMgr.h"
#include "Shader.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ShaderMgr"

static std::mutex g_mutex{};
static ShaderMgr* g_mgr{ nullptr };

static const char* STR_NONE = "none";
static const char* STR_OBJ = "obj";
static const char* STR_CANVAS = "cas";

ShaderMgr* ShaderMgr::get()
{
    if (!g_mgr) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_mgr)
            g_mgr = new ShaderMgr;
    }
    return g_mgr;
}

ShaderMgr::ShaderMgr() : m_shaders() {}

ShaderMgr::~ShaderMgr() 
{ 
    auto itr = m_shaders.begin();
    while (itr != m_shaders.end())
    {
        itr->second.reset();
        itr = m_shaders.erase(itr);
    }
}

const std::shared_ptr<Shader>& ShaderMgr::getShader(ShaderType type)
{
    std::string key(type2Str(type));

    auto itr = m_shaders.find(key);
    if (itr != m_shaders.end())
    {
        const std::shared_ptr<Shader>& shader = itr->second;
        return shader;
    }
    else
    {
        LOG_ERR("fail to find shader[%u][%s]", type, type2Str(type));
        return std::shared_ptr<Shader>(nullptr);
    }
}

void ShaderMgr::init()
{
    std::string key = STR_OBJ;
    std::shared_ptr<Shader> objShader = std::make_shared<Shader>(STR_OBJ, STR_OBJ, STR_OBJ);
    m_shaders.insert(std::pair<std::string, std::shared_ptr<Shader>>(key, objShader));
}

void ShaderMgr::release()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    delete g_mgr;
    g_mgr = nullptr;
}

const char* ShaderMgr::type2Str(ShaderType type)
{
    switch (type)
    {
    case ShaderType::SHADER_OBJ:
    {
        return STR_OBJ;
    }
    case ShaderType::SHADER_CANVAS:
    {
        return STR_CANVAS;
    }
    default:
    {
        return STR_NONE;
    }
    }
}