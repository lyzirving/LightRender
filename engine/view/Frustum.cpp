#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "Frustum.h"
#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Frustum"

Frustum::Frustum(float fov, float aspect, float near, float far) : m_fov(fov), m_aspect(aspect), m_near(near), m_far(far),
                                                                   m_change(true), m_projectMat(1.f)
{
}

Frustum::~Frustum() = default;

void Frustum::calcProjectMat()
{
    if (m_change.load())
    {
        if (m_near <= 0.f || m_far <= 0.f)
        {
            LOG_ERR("err, invalid value, near[%.2f] and far[%.2f]", m_near, m_far);
            assert(0);
        }
        m_projectMat = glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
        m_change.store(false);
    }
}

const glm::mat4& Frustum::getProjectMat()
{
    calcProjectMat();
    return m_projectMat;
}

void Frustum::setFov(float fov)
{
    if (fov != m_fov)
    {
        m_fov = fov;
        m_change.store(true);
        LOG_INFO("fov[%.2f] changes", m_fov);
    }
}

void Frustum::setAspect(float aspect)
{
    if (aspect != m_aspect)
    {
        m_aspect = aspect;
        m_change.store(true);
        LOG_INFO("aspect[%.2f] changes", m_aspect);
    }
}

void Frustum::setNearFar(float near, float far)
{
    if (near != m_near || far != m_far)
    {
        m_near = near;
        m_far = far;
        m_change.store(true);
        LOG_INFO("near[%.2f] and far[%.2f] changes", m_near, m_far);
    }
}