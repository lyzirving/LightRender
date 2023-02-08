#ifndef METAL_H
#define METAL_H

#include "Matl.h"

class Metal : public Matl
{
public:
	Metal(const glm::vec3& color = glm::vec3(0.5f));
	virtual ~Metal();

	virtual bool scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const override;

	void setFluzzy(float fuzzy);

private:
	// a perturbation to make specular reflection blur
	// m_fuzzy ranges [0, 1],  when val is 0, there is no blur, while val is 1.f, the blur is fullest.
	float m_fuzzy;
};

#endif