#ifndef GFX_DEF_H
#define GFX_DEF_H

#include <string>
#include <glm/glm.hpp>

static const std::string U_MAT_MODEL = std::string("u_modelMat");
static const std::string U_MAT_VIEW  = std::string("u_viewMat");
static const std::string U_MAT_PRJ   = std::string("u_prjMat");

static const std::string U_SMP_DIFF = std::string("u_diffuse");
static const std::string U_SMP_SPEC = std::string("u_spec");

enum ShaderType : uint8_t
{
    SHADER_OBJ,
    SHADER_CANVAS,
    SHADER_CNT
};

enum TexType : uint8_t
{
    DIFFUSE,
    SPECULAR,
    TEX_TYPE_COUNT
};

enum DrawMode : uint8_t
{
    MODE_TRIANGLE,
    MODE_LINE,
    MODE_POINT
};

struct Vertex
{
    glm::vec3 m_pos;
    glm::vec3 m_normal;
    glm::vec2 m_tex;

    Vertex() : m_pos(0.f), m_normal(0.f), m_tex(0.f) {}
};

struct Texture
{
public:
    std::string m_path;
    int32_t m_width, m_height, m_channel;
    TexType m_type;
    uint32_t m_texId;

    Texture() : m_path(), m_width(0), m_height(0), m_channel(0), 
                m_type(TEX_TYPE_COUNT), m_texId(0) 
    {
    }
};

struct Material
{
public:
    glm::vec3 m_ka, m_kd, m_ks;
    float m_shine;

    Material() : m_ka(0.f), m_kd(0.f), m_ks(0.f), m_shine(100.f) {}
};

#endif // !GFX_DEF_H

