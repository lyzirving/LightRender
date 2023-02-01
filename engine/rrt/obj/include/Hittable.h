#ifndef HITTABLE_H
#define HITTABLE_H

#include <glm/glm.hpp>

class Ray;

struct HitRecord
{
	bool hit;
	float t;
	glm::vec3 pt;
	// normal should always point outward from the surface
	glm::vec3 n;

	HitRecord() : hit(false), t(0.f), pt(0.f), n(0.f) {}
};

class Hittable
{
public:
	Hittable() {}
	virtual ~Hittable() = default;

	virtual void hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const = 0;
};

#endif
