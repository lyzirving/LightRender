#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class RrtMaterial;

class Sphere : public Hittable
{
public:
	Sphere();
	Sphere(const glm::vec3 & center, const float radius);
	virtual ~Sphere();

	virtual bool boundingBox(AABB& box) const override;
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

private:
	glm::vec3 m_center;
	float m_radius;
};

#endif
