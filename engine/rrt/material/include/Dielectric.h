#ifndef DILECTRIC_H
#define DILECTRIC_H

class RrtTexture;

#include <memory>

#include "RrtMaterial.h"

class Dilectric : public RrtMaterial
{
public:
	Dilectric(float refractIndex, const glm::vec3 &color = glm::vec3(0.5f));
	virtual ~Dilectric();

	virtual bool scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const override;

private:
	std::shared_ptr<RrtTexture> m_color;
	float m_refractIndex;
};

#endif
