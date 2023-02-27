#ifndef RRT_MATERIAL_H
#define RRT_MATERIAL_H

class Ray;
struct HitRecord;

#include <glm/glm.hpp>

class RrtMaterial
{
public:
	RrtMaterial() {}
	virtual ~RrtMaterial() = default;

	virtual bool scatter(const Ray &input, const HitRecord &rec, glm::vec3 &attenuation, Ray &scatterRay) const = 0;
};

#endif
