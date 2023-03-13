#ifndef RRT_DIFFUSE_LIGHT_H
#define RRT_DIFFUSE_LIGHT_H

#include <memory>

class RrtTexture;

#include "RrtMaterial.h"

class DiffuseLight : public RrtMaterial
{
public:
	DiffuseLight(const std::shared_ptr<RrtTexture> &emit);
	virtual ~DiffuseLight();

	virtual glm::vec3 emit(float u, float v, const glm::vec3 p) const override;
	virtual bool scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay, float& pdf) const override;

private:
	std::shared_ptr<RrtTexture> m_emit;
};

#endif