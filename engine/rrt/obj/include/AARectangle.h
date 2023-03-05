#ifndef RRT_PLANE_H
#define RRT_PLANE_H

#include <atomic>

#include "Hittable.h"

/*
* Axis aligned rectangle
*/
class AARectangle : public Hittable
{
public:
	/*
	* front is the +z of the axis aligned rectangle.
	* front must be orthogonal to +x, +y and +z, or the rectangle can not be correctly drawn.
	*/
	AARectangle(const glm::vec3 &center, const glm::vec3 &front, const glm::vec3 &size, const std::shared_ptr<RrtMaterial> &material);
	virtual ~AARectangle();

	virtual bool boundingBox(AABB& box) const override;
	virtual glm::vec3 center() const override;
	virtual bool hit(const Ray& ray, float tStart, float tEnd, HitRecord& record) const override;

	void apply();
	void rotate(float angle, const glm::vec3& axis);
	void resetRotate();
	void setCenter(const glm::vec3& center);
	void setFront(const glm::vec3 &front);
	void setSize(float width, float height, float thickness);

private:
	bool hitPtNormal(const glm::vec3 pt, const glm::vec3 center, glm::vec3 &out) const;

	glm::vec3 m_center;
	glm::vec3 m_lbFrontPt, m_lbBackPt, m_ltFrontPt, m_ltBackPt;
	glm::vec3 m_rbFrontPt, m_rbBackPt, m_rtFrontPt, m_rtBackPt;
	glm::vec3 m_front, m_right, m_up;
	glm::mat4 m_rotateMat;
	std::shared_ptr<AABB> m_aabb;
	float m_width, m_height, m_thickness;
	std::atomic<bool> m_dataChange;
};

#endif
