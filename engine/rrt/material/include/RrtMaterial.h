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
	* @brif emit light for light emitter.
	*/
	virtual glm::vec3 emit(float u, float v, const glm::vec3 p) const { return glm::vec3(0.f); };

	/*
	* @brif reflection or refraction of incident light.
	*       light emitter always return false for this method.
	* @param input:      input ray
	* @param rec:        hit record of input ray on this material
	* @param scatterRay: ray reflected of refracted by this material
	* @param pdf:        probability distribution function of the light
	*/
	virtual bool scatter(const Ray &input, const HitRecord &rec, glm::vec3 &albedo, Ray &scatterRay, float &pdf) const = 0;

	virtual float scatterPdf(const Ray& input, const HitRecord& rec, Ray& scatterRay) const { return 1.f; }
};

#endif
