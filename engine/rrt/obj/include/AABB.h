#ifndef AA_BB_H
#define AA_BB_H

#include <glm/glm.hpp>

class Ray;
struct HitRecord;

class AABB
{
public:
	AABB();
	AABB(const glm::vec3& aa, const glm::vec3& bb);
	virtual ~AABB();

	inline const glm::vec3 AA() const { return m_aa; }
	inline const glm::vec3 BB() const { return m_bb; }
	inline void setAA(const glm::vec3& aa) { m_aa = aa; }
	inline void setBB(const glm::vec3& bb) { m_bb = bb; }

	bool contains(const glm::vec3& pt) const;
	bool hit(const Ray& ray, float &hitTime) const;

	static AABB surroundingBox(const AABB &lhs, const AABB &rhs);

private:
	glm::vec3 m_aa, m_bb;
};

#endif
