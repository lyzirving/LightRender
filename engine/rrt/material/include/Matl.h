#ifndef MATERIAL_H
#define MATERIAL_H

class Ray;
struct HitRecord;

#include <glm/glm.hpp>

class Matl
{
public:
	Matl() {}
	virtual ~Matl() = default;

	virtual bool scatter(const Ray &input, const HitRecord &rec, glm::vec3 &attenuation, Ray &scatterRay) const = 0;
};

#endif
