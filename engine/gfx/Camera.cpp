#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "ViewLib.h"
#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Camera"

Camera::Camera(float camTheta, float camPhi, float camRadius, float viewDirTheta, float viewDirPhi) :
		m_worldUp(0.f, 1.f, 0.f), m_camPos(0.f), m_camRight(0.f), m_camUp(0.f), m_viewDir(0.f),
		m_camPosTheta(camTheta), m_camPosPhi(camPhi), m_camPosRadius(camRadius),
		m_viewDirTheta(viewDirTheta), m_viewDirPhi(viewDirPhi),
		m_viewMat(1.f), m_dataChange(true)
{
}

Camera::~Camera() = default;

void Camera::calcViewMat()
{
    if (m_dataChange.load())
    {
        ViewLib::SCStoCCS(m_viewDirTheta, m_viewDirPhi, 1.f, m_viewDir);
        m_viewDir = glm::normalize(m_viewDir);

        // compute the right and up vector
        m_camRight = glm::normalize(glm::cross(m_viewDir, m_worldUp));
        m_camUp = glm::normalize(glm::cross(m_camRight, m_viewDir));

        ViewLib::SCStoCCS(m_camPosTheta, m_camPosPhi, m_camPosRadius, m_camPos);

        m_viewMat = glm::lookAt(m_camPos, m_camPos + m_viewDir, m_camUp);
        m_dataChange.store(false);
    }
}

void Camera::checkBound()
{
    if (m_camPosPhi > 360.f)
    {
        m_camPosPhi = (float)(int(m_camPosPhi) % 360);
    }
    else if (m_camPosPhi < 0.f)
    {
        m_camPosPhi = 360.f + m_camPosPhi;
    }

    if (m_camPosTheta > 178.f)
    {
        m_camPosTheta = 178.f;
    }
    else if (m_camPosTheta < 2.f)
    {
        m_camPosTheta = 2.f;
    }

    // TODO set the bound of view direction
    if (m_camPosPhi <= 90.f)
    {
        
    }
    else if (m_camPosPhi > 90.f && m_camPosPhi <= 180.f)
    {
        
    }
    else if (m_camPosPhi > 180.f && m_camPosPhi <= 270.f)
    {
        
    }
    else
    {
        
    }

    if (m_camPosTheta <= 90.f)
    {
        
    }
    else
    {
        
    }
}

const glm::mat4& Camera::getViewMat()
{
    calcViewMat();
    return m_viewMat;
}

const glm::vec3& Camera::getCamPos()
{
    calcViewMat();
    return m_camPos;
}

void Camera::setPosition(const glm::vec3& pos)
{
    if (pos != m_camPos)
    {
        glm::vec3 spherical;
        ViewLib::CCStoSCS(pos, spherical);

        m_camPosTheta = spherical.x;
        m_camPosPhi = spherical.y;
        m_camPosRadius = spherical.z;
        checkBound();
        m_dataChange.store(true);
    }
}

void Camera::setPosition(float r, float theta, float phi)
{
    if (r != m_camPosRadius || theta != m_camPosTheta || phi != m_camPosPhi)
    {
        m_camPosTheta = theta;
        m_camPosPhi = phi;
        m_camPosRadius = r;
        checkBound();
        m_dataChange.store(true);
    }
}