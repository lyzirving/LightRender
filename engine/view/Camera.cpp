#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

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
		m_viewMat(1.f), m_dataChange(true), m_stateStack()
{
    // compute view direction at first
    // camera always look at the origin of world by now
    calcViewDirection();
    calcCameraPosition();
}

Camera::~Camera() = default;

void Camera::calcViewMat()
{
    if (m_dataChange.load())
    {
        // compute the right and up vector by view direction and world up
        m_camRight = glm::normalize(glm::cross(m_viewDir, m_worldUp));
        m_camUp = glm::normalize(glm::cross(m_camRight, m_viewDir));

        m_viewMat = glm::lookAt(m_camPos, m_camPos + m_viewDir, m_camUp);
        m_dataChange.store(false);

        //LOG_INFO("cam pos[%f, %f, %f], view dir[%f, %f, %f]", m_camPos.x, m_camPos.y, m_camPos.z, m_viewDir.x, m_viewDir.y, m_viewDir.z);
    }
}

void Camera::calcCameraPosition()
{
    ViewLib::SCStoCCS(m_camPosTheta, m_camPosPhi, m_camPosRadius, m_camPos);
}

void Camera::calcViewDirection()
{
    ViewLib::SCStoCCS(m_viewDirTheta, m_viewDirPhi, 1.f, m_viewDir);
    //m_viewDir = glm::normalize(m_viewDir);
}

void Camera::checkCamPosPhi()
{
    if (m_camPosPhi > 360.f)
    {
        m_camPosPhi = (int)m_camPosPhi % 360;
    }
    else if (m_camPosPhi < 0.f)
    {
        float absPhi = std::abs(m_camPosPhi);
        absPhi = (int)absPhi % 360;
        m_camPosPhi = 360.f - absPhi;
    }
}

void Camera::checkCamPos()
{
    checkCamPosPhi();

    float absTheta = std::abs(m_camPosTheta);
    bool odd = (((int)absTheta / 180) % 2) != 0;

    if (m_camPosTheta > 180.f)
    {
        if (odd)
        {
            m_camPosTheta = 180.f - ((int)absTheta % 180);
            m_camPosPhi += 180.f;
            checkCamPosPhi();
        }
        else
        {
            m_camPosTheta = (float)((int)absTheta % 180);
        }
    }
    else if (m_camPosTheta < 0.f)
    {
        if (odd)
        {
            m_camPosTheta = 180.f - ((int)(absTheta) % 180);
        }
        else
        {
            m_camPosTheta = (float)((int)(absTheta) % 180);
            m_camPosPhi += 180.f;
            checkCamPosPhi();
        }
    }

    calcCameraPosition();
    keepViewDirToOrigin();
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

void Camera::keepViewDirToOrigin()
{
    glm::vec3 viewDir, viewDirSCS;
    viewDir = glm::normalize(-m_camPos);
    ViewLib::CCStoSCS(viewDir, viewDirSCS);

    // get theta and phi of view direction
    m_viewDirTheta = viewDirSCS.x;
    m_viewDirPhi = viewDirSCS.y;

    calcViewDirection();
}

void Camera::popState()
{
    if (m_stateStack.empty())
    {
        LOG_ERR("state stack is empty, err operation");
        assert(0);
    }
    CameraState state = m_stateStack.front();
    m_camPosTheta = state.m_posTheta;
    m_camPosPhi = state.m_posPhi;
    m_camPosRadius = state.m_posRadius;
    m_viewDirTheta = state.m_dirTheta;
    m_viewDirPhi = state.m_dirPhi;
    calcCameraPosition();
    calcViewDirection();
    m_stateStack.pop_front();
    m_dataChange.store(true);
}

void Camera::restoreState()
{
    CameraState state{ m_camPosTheta, m_camPosPhi, m_camPosRadius, m_viewDirTheta, m_viewDirPhi };
    m_stateStack.push_back(state);
}

void Camera::setPosition(float theta, float phi, float r)
{
    if (r != m_camPosRadius || theta != m_camPosTheta || phi != m_camPosPhi)
    {
        m_camPosTheta = theta;
        m_camPosPhi = phi;
        m_camPosRadius = r;
        checkCamPos();
        m_dataChange.store(true);
    }
}

void Camera::setPosition(const glm::vec3& pos)
{
    if (m_camPos != pos)
    {
        glm::vec3 camPosSCS;
        ViewLib::CCStoSCS(pos, camPosSCS);
        m_camPosTheta = camPosSCS.x;
        m_camPosPhi = camPosSCS.y;
        m_camPosRadius = camPosSCS.z;
        checkCamPos();
        m_dataChange.store(true);
    }
}

void Camera::setLookAt(const glm::vec3& lookAt)
{
    // need to be implemented
    m_viewDir = glm::normalize(lookAt - m_camPos);
    glm::vec3 out;
    ViewLib::CCStoSCS(m_viewDir, out);
    m_viewDirTheta = out.x;
    m_viewDirPhi = out.y;
    m_dataChange.store(true);
}