#include "PinholeCamera.h"

PinholeCamera::PinholeCamera() : RrtCamera() {}

PinholeCamera::~PinholeCamera() = default;

Ray PinholeCamera::getRay(float u, float v)
{
	glm::vec3 dst = glm::vec3(u, v, 0.f);
	// homogeneous coordinate, mark w as 1.f to make it a point
	dst = m_invMat * glm::vec4(dst, 1.f);

	Ray ray;
	ray.setOrigin(m_camPos);
	ray.setDirection(dst - m_camPos);

	return ray;
}