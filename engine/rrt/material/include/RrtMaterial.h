#ifndef RRT_MATERIAL_H
#define RRT_MATERIAL_H

class Ray;
struct HitRecord;

#include <glm/glm.hpp>

class RrtMaterial
{
public:
	RrtMaterial(const glm::vec3 &color = glm::vec3(0.5f)) : m_albedo(color) {}
	virtual ~RrtMaterial() = default;

	inline void setColor(float r, float g, float b) { m_albedo = glm::vec3(r, g, b); }

	virtual bool scatter(const Ray &input, const HitRecord &rec, glm::vec3 &attenuation, Ray &scatterRay) const = 0;

protected:
	// m_albedo = radiosity / irradiance
	// radiosity is the amount of light the meterial reflects per area
	// irradiance is the amount of light the meterial absorbs per area
	glm::vec3 m_albedo;
};

#endif
