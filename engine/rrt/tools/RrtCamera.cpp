#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "RrtCamera.h"

RrtCamera::RrtCamera() : m_focalLen(1.f), m_fov(45.f), m_viewportSize(500.f, 500.f),
                         m_camPos(0.f), m_lookAt(0.f, 0.f, -1.f), 
	                     m_camFront(0.f), m_camRight(0.f), m_camUp(0.f),
	                     m_viewMat(1.f), m_prjMat(1.f), m_viewportMat(1.f), m_invMat(1.f)
{
	apply();
}

RrtCamera::~RrtCamera() = default;

void RrtCamera::apply()
{
	calcViewMat();
	calcPrjMat();
	calcViewportMat();
	calcInvMat();
}

void RrtCamera::calcViewMat()
{
	m_camFront = glm::normalize(m_camPos - m_lookAt);
	m_camRight = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_camFront));
	m_camUp = glm::normalize(glm::cross(m_camFront, m_camRight));

	m_viewMat = glm::lookAt(m_camPos, m_camPos - m_camFront, m_camUp);
}

void RrtCamera::calcPrjMat()
{
	m_prjMat = glm::perspective(glm::radians(m_fov), m_viewportSize.x / m_viewportSize.y, m_focalLen, 10.f);
}

void RrtCamera::calcViewportMat()
{
	m_viewportMat = glm::mat4(1.f);

	float width = m_viewportSize.x;
	float height = m_viewportSize.y;
	float d = 1.f;
	m_viewportMat[0][0] = width / 2.f;
	m_viewportMat[1][1] = height / 2.f;
	m_viewportMat[2][2] = d / 2.f;
	m_viewportMat[3][0] = 0 + width / 2.f;
	m_viewportMat[3][1] = 0 + height / 2.f;
	m_viewportMat[3][2] = d / 2.f;
}

void RrtCamera::calcInvMat()
{
	glm::mat4 mat = m_viewportMat * m_prjMat * m_viewMat;
	m_invMat = glm::inverse(mat);
}

Ray RrtCamera::getRay(float u, float v) const
{
	// homogeneous coordinate, mark w as 1.f to make it a point
	glm::vec4 dst = glm::vec4(u, v, 0.f, 1.f);
	dst = m_invMat * dst;

	Ray r;
	r.setOrigin(m_camPos);
	r.setDirection(glm::normalize(glm::vec3(dst.x, dst.y, dst.z) - m_camPos));
	return r;
}

void RrtCamera::setFocalLen(float focal)
{
	m_focalLen = focal;
}

void RrtCamera::setFov(float fov)
{
	m_fov = fov;
}

void RrtCamera::setLookAt(const glm::vec3& at)
{
	m_lookAt = at;
}

void RrtCamera::setPosition(const glm::vec3& pos)
{
	m_camPos = pos;
}

void RrtCamera::setViewportSize(const glm::vec2& size)
{
	m_viewportSize = size;
}