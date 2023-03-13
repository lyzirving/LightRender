#include "DiffuseLight.h"
#include "RrtTexture.h"

DiffuseLight::DiffuseLight(const std::shared_ptr<RrtTexture>& emit) : RrtMaterial(), m_emit(emit) {}

DiffuseLight::~DiffuseLight()
{
	m_emit.reset();
}

glm::vec3 DiffuseLight::emit(float u, float v, const glm::vec3 p) const
{
	return m_emit->value(u, v, p);
}

bool DiffuseLight::scatter(const Ray& input, const HitRecord& rec, glm::vec3& attenuation, Ray& scatterRay, float& pdf) const
{
	return false;
}