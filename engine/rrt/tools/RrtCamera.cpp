#include "RrtCamera.h"

RrtCamera::RrtCamera() : m_aspect(1.f), m_focalLen(1.f), m_pos(0.f), m_imgHeight(2.f), m_imgWidth(0.f)
{
	m_imgWidth = m_imgHeight * m_aspect;
	computeImgLb();
}

RrtCamera::~RrtCamera() = default;

void RrtCamera::computeImgLb()
{
	m_imgLt = m_pos;
	m_imgLt.x -= m_imgWidth / 2.f;
	m_imgLt.y += m_imgHeight / 2.f;
	m_imgLt.z -= m_focalLen;
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