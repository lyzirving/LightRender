#ifndef RRT_PLANE_H
#define RRT_PLANE_H

#include "Hittable.h"

class Plane : public Hittable
{
public:
	Plane(const glm::vec3 &center, const glm::vec3 &front, const std::shared_ptr<RrtMaterial> &material);
	virtual ~Plane();

	virtual bool boundingBox(AABB& box) const override;
	virtual glm::vec3 center() const override;
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

private:
	glm::vec3 m_center;
	glm::vec3 m_front, m_right, m_up;
	float m_width, m_height;
};

#endif
