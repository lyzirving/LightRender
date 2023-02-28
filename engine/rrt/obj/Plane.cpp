#include "Plane.h"

Plane::Plane(const glm::vec3& center, const glm::vec3& front, const std::shared_ptr<RrtMaterial>& material) 
	         : Hittable(material), m_center(center), m_front(front), m_right(0.f), m_up(0.f),
	           m_width(0.f), m_height(0.f)
{
}

Plane::~Plane() = default;

bool Plane::boundingBox(AABB& box) const
{
	return true;
}

glm::vec3 Plane::center() const
{
	return glm::vec3(0.f);
}

bool Plane::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	return true;
}