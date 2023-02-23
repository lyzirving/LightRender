#include <mutex>

#include "GfxShaderMgr.h"
#include "GfxShader.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxShaderMgr"

static std::mutex g_mutex{};
static GfxShaderMgr* g_mgr{ nullptr };

static const char* STR_NONE   = "none";
static const char* STR_OBJ    = "obj";
static const char* STR_CANVAS = "cas";

GfxShaderMgr* GfxShaderMgr::get()
{
    if (!g_mgr) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_mgr)
            g_mgr = new GfxShaderMgr;
    }
    return g_mgr;
}

GfxShaderMgr::GfxShaderMgr() : m_shaders() {}

GfxShaderMgr::~GfxShaderMgr() 
{ 
    auto itr = m_shaders.begin();
    while (itr != m_shaders.end())
    {
        itr->second.reset();
        itr = m_shaders.erase(itr);
    }
}

const std::shared_ptr<GfxShader>& GfxShaderMgr::getShader(GfxShaderType type)
{
    std::string key(type2Str(type));

    auto itr = m_shaders.find(key);
    if (itr != m_shaders.end())
    {
        const std::shared_ptr<GfxShader>& shader = itr->second;
        return shader;
    }
    else
    {
        LOG_ERR("fail to find shader[%u][%s]", type, type2Str(type));
        return std::shared_ptr<GfxShader>(nullptr);
    }
}

void GfxShaderMgr::init()
{
    std::string key = STR_OBJ;
    std::shared_ptr<GfxShader> objShader = std::make_shared<GfxShader>(STR_OBJ, STR_OBJ, STR_OBJ);
    m_shaders.insert(std::pair<std::string, std::shared_ptr<GfxShader>>(key, objShader));

    key = STR_CANVAS;
    std::shared_ptr<GfxShader> casShader = std::make_shared<GfxShader>(STR_CANVAS, STR_CANVAS, STR_CANVAS);
    m_shaders.insert(std::pair<std::string, std::shared_ptr<GfxShader>>(key, casShader));
}

void GfxShaderMgr::release()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    delete g_mgr;
    g_mgr = nullptr;
}

const char* GfxShaderMgr::type2Str(GfxShaderType type)
{
    switch (type)
    {
    case GfxShaderType::SHADER_OBJ:
    {
        return STR_OBJ;
    }
    case GfxShaderType::SHADER_CANVAS:
    {
        return STR_CANVAS;
    }
    default:
    {
        return STR_NONE;
    }
    }
}