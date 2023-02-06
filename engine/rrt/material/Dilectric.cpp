#include <cmath>

#include "Dielectric.h"
#include "Ray.h"
#include "Hittable.h"

Dilectric::Dilectric(float refractIndex) : Matl(glm::vec3(1.f)), m_refractIndex(refractIndex)
{
}

Dilectric::~Dilectric() = default;

glm::vec3 Dilectric::refract(const glm::vec3& input, const glm::vec3& n, float index) const
{
	float cosTheta = (float)std::fmin(glm::dot(-input, n), 1.f);
	glm::vec3 rayPerp = index * (input + cosTheta * n);
	glm::vec3 rayParal = -(float)std::sqrt(std::fabs(1.0 - glm::length(rayPerp))) * n;
	return glm::normalize(rayPerp + rayParal);
}

bool Dilectric::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const
{
	float index = rec.frontFace ? (1.f / m_refractIndex) : m_refractIndex;
	glm::vec3 refractDir = refract(glm::normalize(input.direction()), rec.n, index);

	scatterRay.setOrigin(rec.pt);
	scatterRay.setDirection(refractDir);

	attenuation = m_albedo;
	return true;
}