#include <glm/gtc/matrix_transform.hpp>

#include "Plane.h"
#include "AABB.h"

Plane::Plane(const glm::vec3& center, const glm::vec3& front, float width, float height, float thickness, const std::shared_ptr<RrtMaterial>& material)
	         : Hittable(material), m_center(center), 
	           m_front(glm::normalize(front)), m_right(1.f, 0.f, 0.f), m_up(0.f, 1.f, 0.f),
	           m_rotateMat(1.f), m_aabb(new AABB), 
	           m_width(width), m_height(height), m_thickness(thickness), m_dataChange(true)
{
	apply();
}

Plane::~Plane()
{
	m_aabb.reset();
}

void Plane::apply()
{
	if (m_dataChange.load())
	{
		m_dataChange.store(false);

		m_front = m_rotateMat* glm::vec4(m_front, 0.f);

		glm::vec3 worldUp{ 0.f, 1.f, 0.f };
		m_right = glm::normalize(glm::cross(worldUp, m_front));
		m_up = glm::normalize(glm::cross(m_front, m_right));

		float halfWidth = m_width * 0.5f;
		float halfHeight = m_height * 0.5f;
		float halfThickness = m_thickness * 0.5f;

		glm::vec3 lb = m_center - m_right * halfWidth - m_up * halfHeight;
		glm::vec3 lt = m_center - m_right * halfWidth + m_up * halfHeight;

		glm::vec3 rb = m_center + m_right * halfWidth - m_up * halfHeight;
		glm::vec3 rt = m_center + m_right * halfWidth + m_up * halfHeight;

		glm::vec3 lbFront = lb + m_front * halfThickness;
		glm::vec3 lbBack  = lb - m_front * halfThickness;
		glm::vec3 ltFront = lt + m_front * halfThickness;
		glm::vec3 ltBack  = lt - m_front * halfThickness;

		glm::vec3 rbFront = rb + m_front * halfThickness;
		glm::vec3 rbBack  = rb - m_front * halfThickness;
		glm::vec3 rtFront = rt + m_front * halfThickness;
		glm::vec3 rtBack  = rt - m_front * halfThickness;

		glm::vec3 aa1 = glm::min(glm::min(glm::min(lbFront, lbBack), rbFront), rbBack);
		glm::vec3 aa2 = glm::min(glm::min(glm::min(ltFront, ltBack), rtFront), rtBack);
		m_aabb->setAA(glm::min(aa1, aa2));

		glm::vec3 bb1 = glm::max(glm::max(glm::max(lbFront, lbBack), rbFront), rbBack);
		glm::vec3 bb2 = glm::max(glm::max(glm::max(ltFront, ltBack), rtFront), rtBack);
		m_aabb->setBB(glm::max(bb1, bb2));
	}
}

void Plane::rotate(float angle, const glm::vec3& axis)
{
	glm::mat4 I{ 1.f };
	I = glm::rotate(I, glm::radians(angle), axis);
	m_rotateMat = I * m_rotateMat;
	m_dataChange.store(true);
}

void Plane::resetRotate()
{
	m_rotateMat = glm::mat4(1.f);
	m_dataChange.store(true);
}

bool Plane::boundingBox(AABB& box) const
{
	box = *m_aabb;
	return true;
}

glm::vec3 Plane::center() const
{
	return m_center;
}

bool Plane::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
	return true;
}

void Plane::setCenter(const glm::vec3& center)
{
	m_center = center;
	m_dataChange.store(true);
}

void Plane::setFront(const glm::vec3& front)
{
	m_front = glm::normalize(front);
	m_dataChange.store(true);
}

void Plane::setSize(float width, float height, float thickness)
{
	m_width = width;
	m_height = height;
	m_thickness = thickness;
	m_dataChange.store(true);
}