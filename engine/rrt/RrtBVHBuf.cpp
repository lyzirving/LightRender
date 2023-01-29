#include <GL/glew.h>

#include "RrtBVHBuf.h"
#include "RrtDef.h"

#include "Shader.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtBVHBuf"

RrtBVHBuf::RrtBVHBuf() : RrtBuffer(), m_nodes()
{
    m_slot = RRT_SLOT_BVH;
}

RrtBVHBuf::~RrtBVHBuf()
{
    std::vector<RrtBVHNode>().swap(m_nodes);
}

void RrtBVHBuf::addNodes(const std::vector<RrtBVHNode>& input)
{
    if (!input.empty())
    {
        m_nodes.assign(input.begin(), input.end());
        LOG_INFO("BVH node size[%lu]", m_nodes.size());

        initBuf();

        glBindBuffer(GL_TEXTURE_BUFFER, m_bufId);
        glBufferData(GL_TEXTURE_BUFFER, m_nodes.size() * sizeof(RrtBVHNode), &m_nodes[0], GL_STATIC_DRAW);

        glBindTexture(GL_TEXTURE_BUFFER, m_texId);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_bufId);
    }
}

void RrtBVHBuf::bind(const std::shared_ptr<Shader>& shader, int texUnit)
{
    if (m_nodes.empty())
    {
        LOG_ERR("nodes is empty");
        return;
    }
    RrtBuffer::bind(shader, texUnit);
}