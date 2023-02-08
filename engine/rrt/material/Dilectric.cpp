#include <cmath>

#include "Dielectric.h"
#include "Ray.h"
#include "Hittable.h"

#include "GfxLib.h"

Dilectric::Dilectric(float refractIndex) : Matl(glm::vec3(1.f)), m_refractIndex(refractIndex)
{
}

Dilectric::~Dilectric() = default;

bool Dilectric::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const
{
	float cosTheta = std::fmin(glm::dot(-input.direction(), rec.n), 1.f);
	float sinTheta = std::sqrt(1.f - cosTheta * cosTheta);

	// if rec.frontFace is true, the ray comes from air(1.f) into Dilectric 
	float ratio = rec.frontFace ? (1.f / m_refractIndex) : m_refractIndex;

	bool internalReflection = (ratio * sinTheta) > 1.f;

	glm::vec3 dir = GfxLib::refract(glm::normalize(input.direction()), rec.n, ratio);
	// todo: the total internal reflection has a bug to be fixed
	/*if (internalReflection)
	{
		dir = GfxLib::reflect(glm::normalize(input.direction()), rec.n);
	}
	else
	{
		dir = GfxLib::refract(glm::normalize(input.direction()), rec.n, ratio);
	}*/
	scatterRay.setOrigin(rec.pt);
	scatterRay.setDirection(dir);
	attenuation = m_albedo;
	return true;
}