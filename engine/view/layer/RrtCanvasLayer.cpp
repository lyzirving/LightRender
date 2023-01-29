#include <cassert>
#include <GL/glew.h>

#include "RrtCanvasLayer.h"

#include "Shader.h"
#include "ShaderMgr.h"

#include "RrtDef.h"
#include "BVHBuilder.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtCanvasLayer"

RrtCanvasLayer::RrtCanvasLayer(LayerOrder order) : Layer(LayerType::LAYER_CANVAS, order),
                                                   m_vao(0), m_vbo(0), m_ebo(0), m_bgColor(0xffffffff),
                                                   m_shader(nullptr), m_BVHBuilder(nullptr)
{
    createItems();
}

RrtCanvasLayer::~RrtCanvasLayer()
{
    m_shader.reset();
    m_BVHBuilder.reset();

    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_ebo != 0) glDeleteBuffers(1, &m_ebo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

void RrtCanvasLayer::createItems() 
{
    initMem();

    m_shader = ShaderMgr::get()->getShader(ShaderType::SHADER_CANVAS);
    if (!m_shader)
    {
        LOG_ERR("failed to get rrt canvas shader");
        assert(0);
    }

    std::vector<RrtTriangle> triangles{};
    std::vector<RrtBVHNode> nodes{};

    m_BVHBuilder = std::make_shared<BVHBuilder>("StanfordBunny", true);
    m_BVHBuilder->build(nodes);
    m_BVHBuilder->getTriangles(triangles);
}

void RrtCanvasLayer::drawCall()
{
}

void RrtCanvasLayer::initMem()
{
    RrtCanvasVert vert[4];
    vert[0].m_pos = glm::vec3(-1.f, 1.f, 0.f);
    vert[1].m_pos = glm::vec3(-1.f, -1.f, 0.f);
    vert[2].m_pos = glm::vec3(1.f, 1.f, 0.f);
    vert[3].m_pos = glm::vec3(1.f, -1.f, 0.f);
    uint32_t ind[6]{ 0, 1, 2, 2, 1, 3 };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(RrtCanvasVert), &vert[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), &ind[0], GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    // bind buffer to vao
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    // set the vertex attribute pointer, vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RrtCanvasVert), (void*)nullptr);

    glBindVertexArray(0);
}

void RrtCanvasLayer::update(const std::shared_ptr<ViewTransform>& trans)
{
    drawCall();
}
