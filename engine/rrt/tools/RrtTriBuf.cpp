#include <GL/glew.h>

#include "RrtTriBuf.h"
#include "RrtDef.h"

#include "GfxShader.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtTriBuf"

RrtTriBuf::RrtTriBuf() : RrtBuffer(), m_triangles()
{
    m_slot = RRT_SLOT_TRI;
}

RrtTriBuf::~RrtTriBuf()
{
    std::vector<EncodeTriangle>().swap(m_triangles);
}

void RrtTriBuf::addTriangles(const std::vector<EncodeTriangle>& input)
{
    if (!input.empty())
    {
        m_triangles.assign(input.begin(), input.end());
        LOG_INFO("triangle size[%lu]", m_triangles.size());

        initBuf();

        glBindBuffer(GL_TEXTURE_BUFFER, m_bufId);
        glBufferData(GL_TEXTURE_BUFFER, m_triangles.size() * sizeof(EncodeTriangle), &m_triangles[0], GL_STATIC_DRAW);

        glBindTexture(GL_TEXTURE_BUFFER, m_texId);
        glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, m_bufId);
    }
}

void RrtTriBuf::bind(const std::shared_ptr<GfxShader>& shader, int texUnit)
{
    if (m_triangles.empty())
    {
        LOG_ERR("array is empty");
        return;
    }
    RrtBuffer::bind(shader, texUnit);
}