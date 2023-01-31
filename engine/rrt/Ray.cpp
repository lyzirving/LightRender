#include "Ray.h"

Ray::Ray() : m_orig(0.f), m_dir(0.f) {}

Ray::Ray(const glm::vec3& orig, const glm::vec3& dir) : m_orig(orig), m_dir(dir) {}

glm::vec3 Ray::at(float t)
{
	return m_orig + t * m_dir;
}

void Ray::setOrigin(const glm::vec3& origin)
{
	m_orig = origin;
}

void Ray::setDirection(const glm::vec3& direction)
{
	m_dir = direction;
}