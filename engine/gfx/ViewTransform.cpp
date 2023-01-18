#include "ViewTransform.h"
#include "Camera.h"

ViewTransform::ViewTransform() : m_camera(new Camera)
{
}

ViewTransform::~ViewTransform()
{
    m_camera.reset();
}