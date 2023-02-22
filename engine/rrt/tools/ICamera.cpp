#include <glm/gtc/matrix_transform.hpp>

#include "ICamera.h"

ICamera::ICamera() : m_fov(60.f), m_near(1.f), m_far(10.f),
	                 m_camPos(0.f, 0.f, 4.f), m_lookAt(0.f, 0.f, 0.f), 
                     m_camFront(0.f), m_camRight(0.f), m_camUp(0.f),
					 m_viewportSize(500.f),
	                 m_camMat(1.f), m_prjMat(1.f), m_screenMat(1.f),
	                 m_invMat(1.f), m_camInv(1.f), m_screenPrjInv(1.f),
	                 m_dataChange(true)
{
	apply();
}

ICamera::~ICamera() = default;

void ICamera::apply()
{
	if (m_dataChange.load())
	{
		calcCamMat();
		calcPrjMat();
		calcScreenMat();
		calcInvMat();
		m_dataChange.store(false);
	}
}

void ICamera::calcCamMat()
{
	m_camFront = glm::normalize(m_camPos - m_lookAt);
	m_camRight = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), m_camFront));
	m_camUp = glm::normalize(glm::cross(m_camFront, m_camRight));

	m_camMat = glm::lookAt(m_camPos, m_camPos - m_camFront, m_camUp);
}

void ICamera::calcPrjMat()
{
	m_prjMat = glm::perspective(glm::radians(m_fov), m_viewportSize.x / m_viewportSize.y, m_near, m_far);
}

void ICamera::calcScreenMat()
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

void ICamera::calcInvMat()
{
	glm::mat4 mat = m_screenMat * m_prjMat * m_camMat;
	m_invMat = glm::inverse(mat);

	m_camInv = glm::inverse(m_camMat);

	mat = m_screenMat * m_prjMat;
	m_screenPrjInv = glm::inverse(mat);
}

void ICamera::setLookAt(const glm::vec3& at)
{
	if (m_lookAt != at)
	{
		m_lookAt = at;
		m_dataChange.store(true);
	}
}

void ICamera::setNearFar(float near, float far)
{
	if (m_near != near || m_far != far)
	{
		m_near = near;
		m_far = far;
		m_dataChange.store(true);
	}
}

void ICamera::setPosition(const glm::vec3& pos)
{
	if (m_camPos != pos)
	{
		m_camPos = pos;
		m_dataChange.store(true);
	}
}

void ICamera::setViewportSize(const glm::vec2& size)
{
	if (m_viewportSize != size)
	{
		m_viewportSize = size;
		m_dataChange.store(true);
	}
}