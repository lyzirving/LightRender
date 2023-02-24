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

	inline const glm::vec3 aa() const { return m_aa; }
	inline const glm::vec3 bb() const { return m_bb; }

	bool hit(const Ray& ray, float &enterTime) const;

private:
	glm::vec3 m_aa, m_bb;
};

#endif
