#ifndef DILECTRIC_H
#define DILECTRIC_H

#include "Matl.h"

class Dilectric : public Matl
{
public:
	Dilectric(float refractIndex);
	virtual ~Dilectric();

	virtual bool scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const override;

private:
	glm::vec3 refract(const glm::vec3 &input, const glm::vec3 &n, float index) const;

	float m_refractIndex;
};

#endif
