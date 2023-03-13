#include "LambDiffuse.h"
#include "Ray.h"
#include "Hittable.h"
#include "SolidColor.h"

#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Diffuse"

LambDiffuse::LambDiffuse(const glm::vec3& color) : RrtMaterial(), m_albedo(new SolidColor(color)) {}

LambDiffuse::~LambDiffuse()
{
	m_albedo.reset();
}

bool LambDiffuse::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay, float& pdf) const
{
	if (rec.hit)
	{
		// the direction of reflection ray for diffuse material is random(unknown)
		// so we create a random method to simulate it 
		// if the target point is on the sphere surface, it produces true Lambertian reflection
		// reflected ray for Lambertian reflection has higher probability to be scattered close to normal, as its distribution is uniform.

		// generate a random direction in hemisphere
		glm::vec3 dir = GfxLib::randomInHemisphere(rec.n);
		scatterRay.setOrigin(rec.pt);
		scatterRay.setDirection(dir);
		attenuation = m_albedo->value(rec.u, rec.v, rec.pt);

		// solid angle of a whole sphere is 4 * PI
		// lambert diffuse create light on hemisphere, so it's pdf is 1 / (2 * PI)
		pdf = 1.f / (2.f * GfxLib::PI);

		return true;
	}
	else
	{
		return false;
	}
}

float LambDiffuse::scatterPdf(const Ray& input, const HitRecord& rec, Ray& scatterRay) const
{
	// compute a pdf that is mainly around normal
	float cosVal = glm::dot(rec.n, scatterRay.direction());
	return cosVal < 0.f ? 0 : cosVal / GfxLib::PI;
}