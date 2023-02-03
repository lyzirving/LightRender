#ifndef MATERIAL_H
#define MATERIAL_H

class Ray;
struct HitRecord;

#include <glm/glm.hpp>

class Material
{
public:
	Material() {};
	virtual ~Material() = default;

	virtual bool scatter(const Ray &input, const HitRecord &rec, glm::vec3 &attenuation, Ray &scatterRay) const = 0;
};

#endif
