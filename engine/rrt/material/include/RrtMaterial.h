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

	/*
	* emit light for light emitter
	*/
	virtual glm::vec3 emit(float u, float v, const glm::vec3 p) const { return glm::vec3(0.f); };

	/*
	* reflect of refract light, and notice that light emitter always return false for this method
	*/
	virtual bool scatter(const Ray &input, const HitRecord &rec, glm::vec3 &attenuation, Ray &scatterRay) const = 0;
};

#endif
