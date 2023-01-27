#include "GfxMesh.h"
#include "GfxDef.h"
#include "Shader.h"

#include "SystemUtil.h"

#include "GL/glew.h"

GfxMesh::GfxMesh() : m_initialized(false), m_name(),
                     m_vao(0), m_vbo(0), m_ebo(0),
                     m_vertices(), m_indices(), m_textures(),
                     m_material(new Material)
{
}

GfxMesh::GfxMesh(const char* name) : m_initialized(false), m_name(name),
                                     m_vao(0), m_vbo(0), m_ebo(0),
                                     m_vertices(), m_indices(), m_textures(),
                                     m_material(new Material)
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