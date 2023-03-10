#ifndef GFX_DEF_H
#define GFX_DEF_H

#include <string>
#include <glm/glm.hpp>

static const char *U_MAT_MODEL = "u_modelMat";
static const char* U_MAT_VIEW  = "u_viewMat";
static const char* U_MAT_PRJ   = "u_prjMat";

static const char* U_SMP_DIFF = "u_diffuse";
static const char* U_SMP_SPEC = "u_spec";

static const char* U_BG_COLOR = "u_bgColor";
static const char* U_EYS_POS  = "u_eysPos";

/* Single-precision epsilon. */
#define EPSILON 1e-5f

enum GfxShaderType : uint8_t
{
    SHADER_OBJ,
    SHADER_CANVAS,
    SHADER_CNT
};

enum GfxTexType : uint8_t
{
    DIFFUSE,
    SPECULAR,
    TEX_TYPE_COUNT
};

enum GfxDrawMode : uint8_t
{
    MODE_TRIANGLE,
    MODE_LINE,
    MODE_POINT
};

struct GfxVertex
{
    glm::vec3 m_pos;
    glm::vec3 m_normal;
    glm::vec2 m_tex;

    GfxVertex() : m_pos(0.f), m_normal(0.f), m_tex(0.f) {}
};

struct GfxTexture
{
public:
    std::string m_path;
    int32_t m_width, m_height, m_channel;
    GfxTexType m_type;
    uint32_t m_texId;

    GfxTexture() : m_path(), m_width(0), m_height(0), m_channel(0), 
                m_type(TEX_TYPE_COUNT), m_texId(0) 
    {
    }
};

struct GfxMaterial
{
public:
    glm::vec3 m_ka, m_kd, m_ks;
    float m_shine;

    GfxMaterial() : m_ka(0.f), m_kd(0.f), m_ks(0.f), m_shine(100.f) {}
};

#endif // !GFX_DEF_H

