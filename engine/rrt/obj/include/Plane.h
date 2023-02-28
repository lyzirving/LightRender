#ifndef RRT_PLANE_H
#define RRT_PLANE_H

#include <atomic>

#include "Hittable.h"

class Plane : public Hittable
{
public:
	Plane(const glm::vec3 &center, const glm::vec3 &front, float width, float height, const std::shared_ptr<RrtMaterial> &material);
	virtual ~Plane();

	virtual bool boundingBox(AABB& box) const override;
	virtual glm::vec3 center() const override;
	virtual bool hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const override;

	void apply();
	void rotate(float angle, const glm::vec3& axis);
	void resetRotate();
	void setCenter(const glm::vec3& center);
	void setFront(const glm::vec3 &front);
	void setSize(float width, float height);

private:
	glm::vec3 m_center;
	glm::vec3 m_front, m_right, m_up;
	glm::mat4 m_rotateMat;
	std::shared_ptr<AABB> m_aabb;
	float m_width, m_height, m_thickness;
	std::atomic<bool> m_dataChange;
};

#endif
