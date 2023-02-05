#ifndef LAMB_DIFFUSE_H
#define LAMB_DIFFUSE_H

#include "Matl.h"

// Lambertian diffuse material
class LambDiffuse : public Matl
{
public:
	LambDiffuse(const glm::vec3& color = glm::vec3(0.5f));
	virtual ~LambDiffuse();

	virtual bool scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const override;
};

#endif
