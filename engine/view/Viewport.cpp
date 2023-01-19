#include "Viewport.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Viewport"

Viewport::Viewport() : m_start(0.f), m_size(0.f), m_dist(0.f), m_viewportMat(1.f), m_change(true) {}

Viewport::Viewport(float startX, float startY, float width, float height, float dist) : m_start(startX, startY), m_size(width, height), m_dist(dist), m_viewportMat(1.f), m_change(true)
{
    calcMat();
}

Viewport::~Viewport() = default;

void Viewport::calcMat()
{
    if (m_change.load())
    {
        m_viewportMat = glm::mat4(1.f);
        m_viewportMat[0].x = m_size.x / 2.f;
        m_viewportMat[1].y = m_size.y / 2.f;
        m_viewportMat[2].z = m_dist / 2.f;
        m_viewportMat[3].x = m_start.x + m_size.x / 2.f;
        m_viewportMat[3].y = m_start.y + m_size.y / 2.f;
        m_viewportMat[3].z = m_dist / 2.f;
        m_change.store(false);
    }
}

const glm::mat4& Viewport::getMat()
{
    calcMat();
    return m_viewportMat;
}

void Viewport::setStart(float x, float y)
{
    if (x != m_start.x || y != m_start.y)
    {
        m_start.x = x;
        m_start.y = y;
        m_change.store(true);
        LOG_INFO("start[%.2f, %.2f] changes", m_start.x, m_start.y);
    }
}

void Viewport::setStart(const glm::vec2& start)
{
    if (start.x != m_start.x || start.y != m_start.y)
    {
        m_start = start;
        m_change.store(true);
        LOG_INFO("start[%.2f, %.2f] changes", m_start.x, m_start.y);
    }
}

void Viewport::setSize(float width, float height)
{
    if (width != m_size.x || height != m_size.y)
    {
        m_size.x = width;
        m_size.y = height;
        m_change.store(true);
        LOG_INFO("size[%.2f, %.2f] changes", m_size.x, m_size.y);
    }
}

void Viewport::setSize(const glm::vec2& size)
{
    if (size.x != m_size.x || size.y != m_size.y)
    {
        m_size = size;
        m_change.store(true);
        LOG_INFO("size[%.2f, %.2f] changes", m_size.x, m_size.y);
    }
}

void Viewport::setDist(float dist)
{
    if (dist != m_dist)
    {
        m_dist = dist;
        m_change.store(true);
        LOG_INFO("dist[%.2f] changes", m_dist);
    }
}
