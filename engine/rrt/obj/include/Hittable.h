#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <glm/glm.hpp>

class Ray;
class RrtMaterial;
class AABB;

struct HitRecord
{
	glm::vec3 pt, n;
	float t;
	float u, v;
	int hitInd;
	bool hit, frontFace;
	
	HitRecord() : pt(0.f), n(0.f), t(0.f), u(0.f), v(0.f), hitInd(-1), hit(false), frontFace(false) {}
};

class Hittable
{
public:
	Hittable() : m_matl(nullptr) {}
	Hittable(const std::shared_ptr<RrtMaterial> &material) : m_matl(material) {}
	virtual ~Hittable() { m_matl.reset(); }

	inline const std::shared_ptr<RrtMaterial>& getMatl() const { return m_matl; };
	inline void setMatl(const std::shared_ptr<RrtMaterial>& matl) { m_matl = matl; }

	virtual bool boundingBox(AABB &box) const = 0;
	virtual glm::vec3 center() const = 0;
	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const = 0;

protected:
	std::shared_ptr<RrtMaterial> m_matl;
};

#endif
