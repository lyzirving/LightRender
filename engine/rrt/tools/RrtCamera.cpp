#include "RrtCamera.h"

RrtCamera::RrtCamera() : m_aspect(1.f), m_focalLen(1.f), m_pos(0.f), m_imgHeight(2.f), m_imgWidth(0.f)
{
	m_imgWidth = m_imgHeight * m_aspect;
	computeImgLb();
}

RrtCamera::~RrtCamera() = default;

void RrtCamera::computeImgLb()
{
	m_imgLb = m_pos;
	m_imgLb.x -= m_imgWidth / 2.f;
	m_imgLb.y -= m_imgHeight / 2.f;
	m_imgLb.z -= m_focalLen;
}

Ray RrtCamera::getRay(float u, float v) const
{
	Ray r;
	r.setOrigin(m_pos);
	glm::vec3 dir = m_imgLb;
	dir.x += u * m_imgWidth;
	dir.y += v * m_imgHeight;
	dir = glm::normalize(dir - m_pos);
	r.setDirection(dir);
	return r;
}

void RrtCamera::setAspect(float aspect)
{
	m_aspect = aspect;
	m_imgWidth = m_imgHeight * m_aspect;
	computeImgLb();
}

void RrtCamera::setFocalLen(float focal)
{
	m_focalLen = focal;
	computeImgLb();
}

void RrtCamera::setPosition(const glm::vec3& pos)
{
	m_pos = pos;
	computeImgLb();
}