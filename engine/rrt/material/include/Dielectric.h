#ifndef DILECTRIC_H
#define DILECTRIC_H

#include "RrtMaterial.h"

class Dilectric : public RrtMaterial
{
public:
	Dilectric(float refractIndex);
	virtual ~Dilectric();

	virtual bool scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const override;

private:
	float m_refractIndex;
};

#endif
