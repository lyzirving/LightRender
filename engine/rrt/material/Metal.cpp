#include <cassert>

#include "Metal.h"
#include "Ray.h"
#include "Hittable.h"
#include "SolidColor.h"

#include "GfxLib.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Metal"

Metal::Metal(const glm::vec3& color, float fuzzy) : RrtMaterial(), m_color(new SolidColor(color)), m_fuzzy(fuzzy) {}

Metal::~Metal()
{
	m_color.reset();
}

bool Metal::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay, float& pdf) const
{
	glm::vec3 refDir = GfxLib::reflect(input.direction(), rec.n);

	scatterRay.setOrigin(rec.pt);
	scatterRay.setDirection(refDir + m_fuzzy * GfxLib::randomOnUnitSphere());
	attenuation = m_color->value(rec.u, rec.v, rec.pt);

	// todo: need to compute the pdf of metal

	return (glm::dot(scatterRay.direction(), rec.n) > 0.f);
}

void Metal::setFuzzy(float fuzzy)
{
	m_fuzzy = GfxLib::clamp(fuzzy, 0.f, 1.f);
}