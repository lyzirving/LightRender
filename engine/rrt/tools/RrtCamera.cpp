#include <cmath>

#include "RrtCamera.h"

RrtCamera::RrtCamera() : m_aspect(1.f), m_focalLen(1.f), m_fov(60.f), m_viewportWidth(0.f), m_viewportHeight(0.f), m_viewportLb(0.f),
                         m_camPos(0.f), m_lookAt(0.f, 0.f, -1.f), m_camFront(0.f), m_camRight(0.f), m_camUp(0.f)
{
	computeAxis();
	computeAttr();
	computeLb();
}

RrtCamera::~RrtCamera() = default;

void RrtCamera::computeAxis()
{
	m_camFront = glm::normalize(m_camPos - m_lookAt);
	m_camRight = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_camFront));
	m_camUp = glm::normalize(glm::cross(m_camFront, m_camRight));
}

void RrtCamera::computeAttr()
{
	float half = std::tanf(glm::radians(m_fov / 2.f)) * m_focalLen;
	m_viewportHeight = 2.f * half;
	m_viewportWidth = m_viewportHeight * m_aspect;
}

void RrtCamera::computeLb()
{
	m_viewportLb = m_camPos 
		          - m_camRight * m_viewportWidth * 0.5f 
		          - m_camUp * m_viewportHeight * 0.5f
		          - m_camFront * m_focalLen;
}

Ray RrtCamera::getRay(float u, float v) const
{
	glm::vec3 dir = m_viewportLb;
	dir.x += u * m_viewportWidth;
	dir.y += v * m_viewportHeight;

	Ray r;
	r.setOrigin(m_camPos);
	r.setDirection(glm::normalize(dir - m_camPos));
	return r;
}

void RrtCamera::setAspect(float aspect)
{
	m_aspect = aspect;
	computeAttr();
	computeLb();
}

void RrtCamera::setFocalLen(float focal)
{
	m_focalLen = focal;
	computeAttr();
	computeLb();
}

void RrtCamera::setFov(float fov)
{
	m_fov = fov;
	computeAttr();
	computeLb();
}

void RrtCamera::setLookAt(const glm::vec3& at)
{
	m_lookAt = at;
	computeAxis();
	computeLb();
}

void RrtCamera::setPosition(const glm::vec3& pos)
{
	m_camPos = pos;
	computeAxis();
	computeLb();
}