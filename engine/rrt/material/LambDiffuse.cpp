#include "LambDiffuse.h"
#include "Ray.h"
#include "Hittable.h"

#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Diffuse"

LambDiffuse::LambDiffuse(const glm::vec3& color) : RrtMaterial(color) {}

LambDiffuse::~LambDiffuse() = default;

bool LambDiffuse::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay) const
{
	if (rec.hit)
	{
		// the direction of reflection ray for diffuse material is random(unknown)
		// so we create a random method to simulate it 
		// if the target point is on the sphere surface, it produces true Lambertian reflection
		// reflected ray for Lambertian reflection has higher probability to be scattered close to normal, as its distribution is uniform.
		glm::vec3 dir = rec.n + GfxLib::randomOnUnitSphere();
		if (GfxLib::nearZero(dir))
		{
			LOG_INFO("near zero, use old normal");
			dir = rec.n;
		}
		scatterRay.setOrigin(rec.pt);
		scatterRay.setDirection(dir);
		attenuation = m_albedo;
		return true;
	}
	else
	{
		return false;
	}
}