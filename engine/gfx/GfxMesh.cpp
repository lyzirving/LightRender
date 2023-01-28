#include <GL/glew.h>

#include "GfxMesh.h"
#include "GfxDef.h"
#include "GfxHelper.h"
#include "Shader.h"

#include "SystemUtil.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxMesh"

GfxMesh::GfxMesh() : m_initialized(false), m_name(),
                     m_vao(0), m_vbo(0), m_ebo(0),
                     m_vertices(), m_indices(), m_textures(),
                     m_material(new Material),
                     m_drawMode(DrawMode::MODE_TRIANGLE)
{
}

GfxMesh::GfxMesh(const char* name) : m_initialized(false), m_name(name),
                                     m_vao(0), m_vbo(0), m_ebo(0),
                                     m_vertices(), m_indices(), m_textures(),
                                     m_material(new Material),
                                     m_drawMode(DrawMode::MODE_TRIANGLE)
{
}

GfxMesh::~GfxMesh()
{
    GfxMesh::freeMem();

    m_material.reset();
    std::vector<Vertex>().swap(m_vertices);
    std::vector<uint32_t>().swap(m_indices);

    auto itr = m_textures.begin();
    while (itr != m_textures.end())
    {

        itr = m_textures.erase(itr);
    }
    std::vector<Texture>().swap(m_textures);
}

void GfxMesh::bind(bool force)
{
    if (force)
        goto setup;

    if (m_initialized)
        return;

    if (m_vertices.empty() || m_indices.empty())
        return;

setup:
    GfxMesh::createMem();

    // load vertex data into array buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    // load indices data into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    // **********finish create buffer **********

    glBindVertexArray(m_vao);
    // bind buffer to vao
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    // set the vertex attribute pointer, vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)nullptr);
    // vertex normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
    // texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tex));

    glBindVertexArray(0);

    LOG_INFO("init mesh[%s], vao[%u], vbo[%u], ebo[%u]", m_name.c_str(), m_vao, m_vbo, m_ebo);

    m_initialized = true;
}

void GfxMesh::createMem()
{
    if (m_vao == 0) glGenVertexArrays(1, &m_vao);
    if (m_vbo == 0) glGenBuffers(1, &m_vbo);
    if (m_ebo == 0) glGenBuffers(1, &m_ebo);
}

void GfxMesh::freeMem()
{
    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_ebo != 0) glDeleteBuffers(1, &m_ebo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

void GfxMesh::draw(const std::shared_ptr<Shader>& shader)
{
    for (int32_t i = 0; i < m_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].m_texId);
        TexType type = m_textures[i].m_type;
        std::string sampler = (type == TexType::DIFFUSE ? U_SMP_DIFF : U_SMP_SPEC);
        shader->setInt(sampler, i);
    }
    glBindVertexArray(m_vao);
    glDrawElements(getGlDrawMode(), m_indices.size(), GL_UNSIGNED_INT, nullptr);
    GfxHelper::checkGlErr("mesh[%s] draw err", m_name.c_str());
    glBindVertexArray(0);
}

uint32_t GfxMesh::getGlDrawMode()
{
    switch (m_drawMode)
    {
    case DrawMode::MODE_LINE:
        return GL_LINES;
    case DrawMode::MODE_POINT:
        return GL_POINTS;
    case DrawMode::MODE_TRIANGLE:
    default:
        return GL_TRIANGLES;
    }
}

std::string GfxMesh::getMeshInfo()
{
    return SystemUtil::format("%s, vertetics size[%d], indices size[%d], texture size[%d]", 
                                          m_name, m_vertices.size(), m_indices.size(), m_textures.size());
}

void GfxMesh::pushBackVert(Vertex&& vert)
{
    m_vertices.push_back(std::move(vert));
}

void GfxMesh::pushBackIndices(uint32_t ind)
{
    m_indices.push_back(ind);
}

void GfxMesh::pushBackTexVec(const std::vector<Texture>& texVec)
{
    for (auto& item : texVec)
    {
        m_textures.emplace_back(item);
    }
}