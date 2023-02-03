#ifndef LAMB_DIFFUSE_H
#define LAMB_DIFFUSE_H

#include "Material.h"

// Lambertian diffuse material
class LambDiffuse : public Material
{
public:
	LambDiffuse(const glm::vec3 &albedo = glm::vec3(0.5f));
	virtual ~LambDiffuse();

	virtual bool scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const override;

private:
	// m_albedo = radiosity / irradiance
	// radiosity is the amount of light the meterial reflects per area
	// irradiance is the amount of light the meterial absorbs per area
	glm::vec3 m_albedo;
};

#endif
