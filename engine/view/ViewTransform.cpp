#include "ViewTransform.h"
#include "Camera.h"
#include "Frustum.h"
#include "Viewport.h"

ViewTransform::ViewTransform() : m_camera(new Camera), m_frustum(new Frustum), m_viewport(new Viewport)
{
}

ViewTransform::~ViewTransform()
{
    m_camera.reset();
    m_frustum.reset();
    m_viewport.reset();
}

const glm::mat4& ViewTransform::getViewMat()
{
    return m_camera->getViewMat();
}

const glm::mat4& ViewTransform::getProjectMat()
{
    return m_frustum->getProjectMat();
}

void ViewTransform::setViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
    m_viewport->setStart(float(x), float(y));
    m_viewport->setSize(float(width), float(height));

    m_frustum->setAspect(float(width) / float(height));
}