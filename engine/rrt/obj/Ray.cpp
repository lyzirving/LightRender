#include "Ray.h"

Ray::Ray() : m_orig(0.f), m_dir(0.f) {}

Ray::Ray(const Ray& other) : m_orig(other.origin()), m_dir(other.direction()) {}

Ray::Ray(const glm::vec3& orig, const glm::vec3& dir) : m_orig(orig), m_dir(glm::normalize(dir)) {}

Ray& Ray::operator=(const Ray& other)
{
	if (this != &other)
	{
		this->m_orig = other.m_orig;
		this->m_dir = other.m_dir;
	}
	return *this;
}

glm::vec3 Ray::at(float t) const
{
	return m_orig + t * m_dir;
}

void Ray::setOrigin(const glm::vec3& origin)
{
	m_orig = origin;
}

void Ray::setDirection(const glm::vec3& direction)
{
	m_dir = glm::normalize(direction);
}