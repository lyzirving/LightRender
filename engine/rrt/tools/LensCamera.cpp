#include <cmath>

#include "LensCamera.h"

#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LensCamera"

LensCamera::LensCamera() : ICamera(), m_focalDist(0.4f), m_distI(0.6f), m_distO(0.f), m_apertureRadius(0.3f)
{
	// The Thin Lens Equation: 1 / f = 1 / zo + 1 / zi
	m_distO = m_focalDist * m_distI / std::abs(m_distI - m_focalDist);
	LOG_INFO("focal dist[%f], dist from lens to sensor[%f], dist from lens to object[%f]", m_focalDist, m_distI, m_distO);
}

LensCamera::~LensCamera() = default;

Ray LensCamera::getRay(float u, float v)
{
	// viewport space
	glm::vec3 screenPt = glm::vec3(u, v, 0.f);

	// camera space
	glm::vec3 sensorCenter = glm::vec3(0.f, 0.f, -m_near);
	glm::vec3 lensCenter = glm::vec3(0.f, 0.f, -m_near - m_distI);
	glm::vec3 ptOnSensor, ptOnLens;
	glm::vec3 dirInCam;

	// world space
	glm::vec3 origin, crossPt;

	// make homogeneous coordinate as 1.f to represent for a point
	// pt being in camera space is located on near plane(z is -m_near), and near plane is our sensor
	ptOnSensor = m_screenPrjInv * glm::vec4(screenPt, 1.f);
	// notice that the lens will flip the image
	ptOnSensor.x *= -1.f;
	ptOnSensor.y *= -1.f;

	ptOnLens = lensCenter + glm::vec3(GfxLib::random(0.f, m_apertureRadius), GfxLib::random(0.f, m_apertureRadius), 0.f);
	dirInCam = glm::normalize(lensCenter - ptOnSensor);

	float t = m_distO / std::abs(dirInCam.z);
	crossPt = lensCenter + dirInCam * t;
	crossPt = m_camInv * glm::vec4(crossPt, 1.f);
	origin = m_camInv * glm::vec4(ptOnLens, 1.f);

	Ray ray;
	ray.setOrigin(origin);
	ray.setDirection(crossPt - origin);
	return ray;
}

float LensCamera::clearObjDist()
{
	return m_near + m_distI + m_distO;
}