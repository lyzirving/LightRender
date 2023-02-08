#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include <glm/glm.hpp>

class Ray;
class Matl;

struct HitRecord
{
	bool hit, frontFace;
	float t;
	int hitInd;
	glm::vec3 pt;
	glm::vec3 n;

	HitRecord() : hit(false), frontFace(false), t(0.f), hitInd(-1), pt(0.f), n(0.f) {}
};

class Hittable
{
public:
	Hittable() : m_matl(nullptr) {}
	virtual ~Hittable() { m_matl.reset(); }

	inline const std::shared_ptr<Matl>& getMatl() const { return m_matl; };
	inline void setMatl(const std::shared_ptr<Matl>& matl) { m_matl = matl; }

	virtual bool hit(const Ray &ray, float tMin, float tMax, HitRecord &record) const = 0;

protected:
	std::shared_ptr<Matl> m_matl;
};

#endif
