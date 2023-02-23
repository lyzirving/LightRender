#include <cassert>
#include <GL/glew.h>

#include "RrtCanvasLayer.h"
#include "RrtTriBuf.h"
#include "RrtBVHBuf.h"
#include "RrtDef.h"
#include "BVHBuilder.h"

#include "ViewTransform.h"
#include "Camera.h"

#include "GfxShader.h"
#include "GfxShaderMgr.h"
#include "GfxDef.h"

#include "ViewDef.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtCanvasLayer"

RrtCanvasLayer::RrtCanvasLayer(LayerOrder order) : Layer(LayerType::LAYER_CANVAS, order),
                                                   m_vao(0), m_vbo(0), m_ebo(0), m_bgColor(0xffffffff),
                                                   m_shader(nullptr), m_BVHBuilder(nullptr),
                                                   m_triBuf(new RrtTriBuf), m_BVHBuf(new RrtBVHBuf)
{
    createItems();
}

RrtCanvasLayer::~RrtCanvasLayer()
{
    m_shader.reset();

    m_BVHBuilder.reset();
    m_triBuf.reset();
    m_BVHBuf.reset();

    if (m_vbo != 0) glDeleteBuffers(1, &m_vbo);
    if (m_ebo != 0) glDeleteBuffers(1, &m_ebo);
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
}

void RrtCanvasLayer::createItems() 
{
    initMem();

    m_shader = GfxShaderMgr::get()->getShader(GfxShaderType::SHADER_CANVAS);
    if (!m_shader)
    {
        LOG_ERR("failed to get rrt canvas shader");
        assert(0);
    }

    std::vector<EncodeTriangle> triangles{};
    std::vector<EncodeBVH> nodes{};

    m_BVHBuilder = std::make_shared<BVHBuilder>("StanfordBunny", true);
    m_BVHBuilder->build(nodes);
    m_BVHBuilder->getTriangles(triangles);

    m_triBuf->addTriangles(triangles);
    m_BVHBuf->addNodes(nodes);
}

void RrtCanvasLayer::drawCall(const std::shared_ptr<ViewTransform>& trans)
{
    const std::shared_ptr<Camera> &cam = trans->camera();
    const glm::mat4 &viewMat = cam->getViewMat();
    const glm::vec3 &eyePos = cam->getCamPos();

    glm::mat4 invViewMat = glm::inverse(viewMat);

    m_shader->setMat4(U_INV_VIEW_MT, invViewMat);
    m_shader->setFloat(U_FOCAL_LEN, 1.5f);
    m_shader->setVec3(U_EYS_POS, eyePos);

    m_shader->setVec4(U_BG_COLOR, R_COMP(m_bgColor), G_COMP(m_bgColor), B_COMP(m_bgColor), A_COMP(m_bgColor));
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
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
    // if the iteration in fragment shader for each pixel is too large, performance problem will occur.
    // if iteration count is out of driver's limit, gpu will recognize it as dead lock, and crash the application.
    m_shader->use(true);
    m_triBuf->bind(m_shader, 0);
    m_shader->setInt(U_TRI_CNT, m_triBuf->triangleCnt());

    m_BVHBuf->bind(m_shader, 1);
    m_shader->setInt(U_BVH_CNT, m_BVHBuf->nodesCnt());

    drawCall(trans);

    m_shader->use(false);
}
