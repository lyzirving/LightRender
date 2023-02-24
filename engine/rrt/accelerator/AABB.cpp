#include <cmath>

#include "AABB.h"
#include "Ray.h"

AABB::AABB() : m_aa(0.f), m_bb(0.f) {}

AABB::AABB(const glm::vec3& aa, const glm::vec3& bb) : m_aa(aa), m_bb(bb) {}

AABB::~AABB() = default;

bool AABB::hit(const Ray& ray, float& enterTime) const
{
	glm::vec3 dir = ray.direction();
	glm::vec3 invDir = 1.f / dir;

	glm::vec3 near = (m_aa - ray.origin()) * invDir;
	glm::vec3 far  = (m_bb - ray.origin()) * invDir;

	glm::vec3 tMin = glm::min(near, far);
	glm::vec3 tMax = glm::max(near, far);

	float t0 = std::fmaxf(tMin.x, std::fmaxf(tMin.y, tMin.z));
	float t1 = std::fminf(tMax.x, std::fminf(tMax.y, tMax.z));
	
	if (t0 >= 0.f && t0 < t1)
	{
		enterTime = t0;
		return true;
	}
	else
	{
		return false;
	}
}