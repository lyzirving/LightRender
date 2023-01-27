#ifdef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <GL/glew.h>
#include <mutex>
#include <cassert>

#include "GfxHelper.h"
#include "GfxDef.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxHelper"

static std::mutex g_mutex{};
static GfxHelper* g_helper{ nullptr };

GfxHelper::GfxHelper() : m_texMap()
{
}

GfxHelper::~GfxHelper()
{
    auto itr = m_texMap.begin();
    while (itr != m_texMap.end())
    {
        if (itr->second.m_texId != 0)
        {

        }
        itr = m_texMap.erase(itr);
    }
}

uint32_t GfxHelper::colorFormat(int component)
{
    if (component == 1)
        return GL_RED;
    else if (component == 2)
        return GL_RG;
    else if (component == 3)
        return GL_RGB;
    else
        return GL_RGBA;
}

bool GfxHelper::checkGlErr(const char* fmt, ...)
{
    GLenum ret = glGetError();
    if (ret == GL_NO_ERROR)
    {
        return true;
    }
    else
    {
        char buf[512] = { 0 };
        va_list ap;
        va_start(ap, fmt);
        std::vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
        va_end(ap);
        LOG_ERR("%s, err[0x%x]", buf, ret);
        return false;
    }
}

GfxHelper* GfxHelper::get()
{
    if (!g_helper) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (!g_helper)
            g_helper = new GfxHelper;
    }
    return g_helper;
}

bool GfxHelper::loadTex(const std::string& path, uint32_t& texId,
                        int32_t& width, int32_t& height, int32_t& channel)
{
    auto itr = m_texMap.find(path);
    if (itr != m_texMap.end())
    {
        Texture& tex = itr->second;
        texId = itr->second.m_texId;
        width = itr->second.m_width;
        height = itr->second.m_height;
        channel = itr->second.m_channel;
        LOG_INFO("texture exists[%s], use cache. tex id[%u], size[%d, %d], channel[%d]", 
                 path.c_str(), texId, width, height, channel);
        return true;
    }

    bool ret{ false };
    Texture tex;
    tex.m_path = path;
    unsigned char* data = stbi_load(path.c_str(), &tex.m_width, &tex.m_height, &tex.m_channel, 0);
    if (data)
    {
        GLenum format = colorFormat(tex.m_channel);

        glGenTextures(1, &tex.m_texId);
        glBindTexture(GL_TEXTURE_2D, tex.m_texId);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, format, tex.m_width, tex.m_height, 0, format, GL_UNSIGNED_BYTE, data);

        if (checkGlErr("fail to load texture[%s]", path.c_str()))
        {
            //success, cache the texture
            texId = tex.m_texId;
            width = tex.m_width;
            height = tex.m_height;
            channel = tex.m_channel;
            m_texMap.insert(std::pair<std::string, Texture>(path, tex));
            ret = true;
            LOG_INFO("succeed to load texture. size[%d, %d], channel[%d], id[%u], path[%s]", 
                     width, height, channel, texId, path.c_str());
        }
    }
    else
    {
        LOG_ERR("fail to load img from[%s]", path.c_str());
    }
    stbi_image_free(data);
    return ret;
}

void GfxHelper::release()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    delete g_helper;
    g_helper = nullptr;
}

