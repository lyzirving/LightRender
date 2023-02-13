#include <cmath>

#include "Dielectric.h"
#include "Ray.h"
#include "Hittable.h"

#include "GfxLib.h"
#include "GfxDef.h"

Dilectric::Dilectric(float refractIndex) : Matl(glm::vec3(1.f)), m_refractIndex(refractIndex)
{
}

Dilectric::~Dilectric() = default;

bool Dilectric::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const
{
	glm::vec3 dir;
	bool totalInternalReflection{ false };
	if (rec.frontFace)
	{
		// ray is from air to inside the surface, only refraction happens
		float ratio = 1.f / m_refractIndex;
		dir = GfxLib::refract(glm::normalize(input.direction()), rec.n, ratio);
	}
	else
	{
		// ray is from inside the surface to ait, need to think of total internal reflection
		float ratio = m_refractIndex;

		float cosTheta = std::fmin(glm::dot(-input.direction(), rec.n), 1.f);
		float sinTheta = std::sqrt(1.f - cosTheta * cosTheta);

		totalInternalReflection = (ratio * sinTheta) > 1.f;

		/*if (totalInternalReflection)
		{
			dir = GfxLib::reflect(glm::normalize(input.direction()), rec.n);
		}
		else
		{
			dir = GfxLib::refract(glm::normalize(input.direction()), rec.n, ratio);
		}*/
		dir = GfxLib::refract(glm::normalize(input.direction()), rec.n, ratio);
	}
	scatterRay.setOrigin(totalInternalReflection ? (rec.pt + rec.n * EPSILON) : rec.pt);
	scatterRay.setDirection(dir);
	attenuation = m_albedo;
	return true;
}