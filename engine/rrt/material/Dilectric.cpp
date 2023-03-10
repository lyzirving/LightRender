#include <cmath>

#include "Dielectric.h"
#include "Ray.h"
#include "SolidColor.h"
#include "Hittable.h"

#include "GfxLib.h"
#include "GfxDef.h"

Dilectric::Dilectric(float refractIndex, const glm::vec3& color) : RrtMaterial(), m_color(new SolidColor(color)), m_refractIndex(refractIndex)
{
}

Dilectric::~Dilectric()
{
	m_color.reset();
}

bool Dilectric::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay, float& pdf) const
{
	// judge whether ray travels from air to inner surface or on the other hand
	float ratio = rec.frontFace ? (1.f / m_refractIndex) : m_refractIndex;

	float cosTheta = std::fmin(glm::dot(-input.direction(), rec.n), 1.f);
	float sinTheta = std::sqrt(1.f - cosTheta * cosTheta);

	bool totalInternalReflection = (ratio * sinTheta) > 1.f;
	bool reflectance = GfxLib::reflectance(cosTheta, ratio) > GfxLib::random();

	glm::vec3 dir;
	// todo: if total internal reflection is enabled, the dilectric sphere will have a black edge which is not correct
	if (reflectance/* || totalInternalReflection*/)
	{
		dir = GfxLib::reflect(input.direction(), rec.n);
	}
	else
	{
		dir = GfxLib::refract(input.direction(), rec.n, ratio);
	}

	scatterRay.setOrigin(rec.pt);
	scatterRay.setDirection(dir);
	attenuation = m_color->value(rec.u, rec.v, rec.pt);

	// todo: need to compute the pdf of Dilectric

	return true;
}