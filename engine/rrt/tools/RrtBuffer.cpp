#include <GL/glew.h>

#include "RrtBuffer.h"

#include "GfxShader.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtBuffer"

RrtBuffer::RrtBuffer() : m_slot(), m_texId(0), m_bufId(0) {}

RrtBuffer::~RrtBuffer()
{
    if (m_bufId != 0)
        glDeleteBuffers(1, &m_bufId);

    if (m_texId != 0)
        glDeleteTextures(1, &m_texId);

    m_bufId = 0;
    m_texId = 0;
}

void RrtBuffer::bind(const std::shared_ptr<GfxShader>& shader, int texUnit)
{
    if (!shader)
    {
        return;
    }
    if (m_slot.empty())
    {
        LOG_ERR("attr slot is empty");
        return;
    }
    if (m_texId == 0 || m_bufId == 0)
    {
        LOG_ERR("buf[%s] invalid id, tex[%u], buf[%u]", m_slot.c_str(), m_texId, m_bufId);
        return;
    }
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_BUFFER, m_texId);
    shader->setInt(m_slot, texUnit);
}

void RrtBuffer::initBuf()
{
    if (m_bufId == 0)
        glGenBuffers(1, &m_bufId);
    if (m_texId == 0)
        glGenTextures(1, &m_texId);
}