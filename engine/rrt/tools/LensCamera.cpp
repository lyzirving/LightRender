#include <cmath>

#include "LensCamera.h"

#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LensCamera"

LensCamera::LensCamera() : ICamera(), m_focalDist(0.4f), m_distLensToSensor(0.6f), m_distLensToSubject(0.f), m_apertureRadius(0.5f) 
{
	m_distLensToSubject = m_focalDist * m_distLensToSensor / std::abs(m_distLensToSensor - m_focalDist);
	LOG_INFO("focal dist[%f], lens to sensor[%f], lens to subject[%f]", m_focalDist, m_distLensToSensor, m_distLensToSubject);
}

LensCamera::~LensCamera() = default;

Ray LensCamera::getRay(float u, float v)
{
	// viewport space
	glm::vec3 screenPt = glm::vec3(u, v, 0.f);

	// camera space
	glm::vec3 sensorCenter = glm::vec3(0.f, 0.f, -m_near);
	glm::vec3 lensCenter = glm::vec3(0.f, 0.f, -m_near - m_distLensToSensor);
	glm::vec3 ptOnSensor, ptOnLens;
	glm::vec3 dirInCam;

	// world space
	glm::vec3 origin, crossPt;

	// make homogeneous coordinate as 1.f to represent for a point
	// pt being in camera space is located on near plane(z is -m_near), and near plane is our sensor
	ptOnSensor = glm::vec3(m_screenPrjInv * glm::vec4(screenPt, 1.f));
	ptOnLens = lensCenter + glm::vec3(GfxLib::random(0.f, m_apertureRadius), GfxLib::random(0.f, m_apertureRadius), 0.f);
	dirInCam = glm::normalize(lensCenter - ptOnSensor);

	float t = m_distLensToSubject / std::abs(dirInCam.z);
	crossPt = lensCenter + dirInCam * t;
	crossPt = m_camInv * glm::vec4(crossPt, 1.f);
	origin = m_camInv * glm::vec4(ptOnLens, 1.f);

	Ray ray;
	ray.setOrigin(origin);
	ray.setDirection(crossPt - origin);
	return ray;
}