#define STB_IMAGE_STATIC
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

uint32_t GfxHelper::createProgram(const char* vert, const char* frag, const char* geo)
{
    GLuint vertShader{ 0 };
    GLuint fragShader{ 0 };
    GLuint geoShader{ 0 };
    GLuint program{ 0 };
    GLint linkStatus{ GL_FALSE };

    bool compileGeo = (geo != nullptr) && (std::strlen(geo) != 0);
    if (vert == nullptr || std::strlen(vert) == 0)
    {
        LOG_ERR("input vertex shader is null");
        goto err;
    }
    if (frag == nullptr || std::strlen(frag) == 0)
    {
        LOG_ERR("input fragment shader is null");
        goto err;
    }
    vertShader = createShader(GL_VERTEX_SHADER, vert);
    if (!vertShader)
    {
        LOG_ERR("fail to compile vertex shader %s", vert);
        goto err;
    }
    fragShader = createShader(GL_FRAGMENT_SHADER, frag);
    if (!fragShader)
    {
        LOG_ERR("fail to compile fragment shader %s", frag);
        goto err;
    }
    if (compileGeo)
    {
        geoShader = createShader(GL_GEOMETRY_SHADER, geo);
        if (!geoShader)
        {
            LOG_ERR("fail to compile geometry shader %s", geo);
            goto err;
        }
    }

    program = glCreateProgram();
    if (!program)
    {
        checkGlErr("fail to create program");
        goto err;
    }
    glAttachShader(program, vertShader);
    if (!checkGlErr("fail to attach vertex shader"))
        goto err;

    glAttachShader(program, fragShader);
    if (!checkGlErr("fail to attach fragment shader"))
        goto err;

    if (compileGeo)
    {
        glAttachShader(program, geoShader);
        if (!checkGlErr("fail to attach geometry shader"))
            goto err;
    }
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        LOG_ERR("fail to link program");
        logShaderInfo(program);
        glDeleteProgram(program);
        goto err;
    }
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
    if (compileGeo)
        glDeleteShader(geoShader);
    return program;

err:
    if (vertShader)
        glDeleteShader(vertShader);
    if (fragShader)
        glDeleteShader(fragShader);
    if (geoShader)
        glDeleteShader(geoShader);
    return 0;
}

uint32_t GfxHelper::createShader(uint32_t type, const char* source)
{
    //glCreateShader return zero when fail
    GLuint shader = glCreateShader(type);
    if (shader)
    {
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        GLint compileStatus = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
        logShaderInfo(shader);
        if (compileStatus != GL_TRUE)
        {
            glDeleteShader(shader);
            shader = 0;
        }
    }
    else
    {
        LOG_ERR("fail to create shader for type(0x%x), err code(0x%x)", type, glGetError());
    }
    return shader;
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

void GfxHelper::logShaderInfo(uint32_t shader)
{
    if (shader == 0)
    {
        LOG_ERR("invalid input shader[0]");
        return;
    }
    GLint infoLen{ 0 };
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
    if (infoLen)
    {
        char* info = static_cast<char*>(std::malloc(sizeof(char) * infoLen));
        if (info)
        {
            glGetShaderInfoLog(shader, infoLen, nullptr, info);
            LOG_INFO("shader(%u) status: %s", shader, info);
            std::free(info);
        }
        else
        {
            LOG_ERR("fail to malloc memory for shader(%u) info log", shader);
        }
    }
}

void GfxHelper::release()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    delete g_helper;
    g_helper = nullptr;
}

