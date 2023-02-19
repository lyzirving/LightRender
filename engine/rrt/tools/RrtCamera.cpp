#include <cmath>
#include <cassert>
#include <glm/gtc/matrix_transform.hpp>

#include "RrtCamera.h"
#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtCamera"

RrtCamera::RrtCamera() : m_focalLen(1.f), m_fov(45.f), m_apertureLen(0.f), m_viewportSize(500.f, 500.f),
                         m_camPos(0.f), m_lookAt(0.f, 0.f, -1.f), 
	                     m_camFront(0.f), m_camRight(0.f), m_camUp(0.f),
	                     m_camMat(1.f), m_prjMat(1.f), m_screenMat(1.f), m_invMat(1.f),
	                     m_camInv(1.f), m_screenPrjInv(1.f)
{
	apply();
}

RrtCamera::~RrtCamera() = default;

void RrtCamera::apply()
{
	calcCamMat();
	calcPrjMat();
	calcScreenMat();
	calcInvMat();
}

void RrtCamera::calcCamMat()
{
	m_camFront = glm::normalize(m_camPos - m_lookAt);
	m_camRight = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_camFront));
	m_camUp = glm::normalize(glm::cross(m_camFront, m_camRight));

	m_camMat = glm::lookAt(m_camPos, m_camPos - m_camFront, m_camUp);
}

void RrtCamera::calcPrjMat()
{
	m_prjMat = glm::perspective(glm::radians(m_fov), m_viewportSize.x / m_viewportSize.y, m_focalLen, 10.f);
}

void RrtCamera::calcScreenMat()
{
	m_screenMat = glm::mat4(1.f);

	float width = m_viewportSize.x;
	float height = m_viewportSize.y;
	float d = 1.f;
	m_screenMat[0][0] = width / 2.f;
	m_screenMat[1][1] = height / 2.f;
	m_screenMat[2][2] = d / 2.f;
	m_screenMat[3][0] = 0 + width / 2.f;
	m_screenMat[3][1] = 0 + height / 2.f;
	m_screenMat[3][2] = d / 2.f;
}

void RrtCamera::calcInvMat()
{
	glm::mat4 mat = m_screenMat * m_prjMat * m_camMat;
	m_invMat = glm::inverse(mat);

	m_camInv = glm::inverse(m_camMat);

	mat = m_screenMat * m_prjMat;
	m_screenPrjInv = glm::inverse(mat);
}

Ray RrtCamera::getRay(float u, float v) const
{
	// homogeneous coordinate, mark w as 1.f to make it a point
	glm::vec4 dst = glm::vec4(u, v, 0.f, 1.f);
	Ray r;

	float radius = m_apertureLen * 0.5f;
	if (GfxLib::nearZero(radius))
	{
		dst = m_invMat * dst;
		r.setOrigin(m_camPos);
		r.setDirection(glm::vec3(dst.x, dst.y, dst.z) - m_camPos);
	}
	else
	{
		glm::vec4 tmp = m_screenPrjInv * dst;

		glm::vec3 dir = glm::normalize(glm::vec3(tmp));
		float t = m_focalLen / std::abs(dir.z);

		glm::vec3 originCamSpace = glm::vec3(GfxLib::random(0.f, radius), GfxLib::random(0.f, radius), 0.f);
		glm::vec3 dstCamSpace = originCamSpace + dir * t;

		tmp = m_camInv * glm::vec4(originCamSpace, 1.f);
		glm::vec3 originWorld = glm::vec3(tmp);

		tmp = m_camInv * glm::vec4(dstCamSpace, 1.f);
		glm::vec3 dstWorld = glm::vec3(tmp);

		r.setOrigin(originWorld);
		r.setDirection(dstWorld - originWorld);
	}
	return r;
}

void RrtCamera::setApertureLen(float len)
{
	if (len < 0.f)
	{
		LOG_ERR("invalid input aperture length[%f]", len);
		assert(0);
	}
	m_apertureLen = len;
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