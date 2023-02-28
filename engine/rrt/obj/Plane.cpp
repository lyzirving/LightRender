#include <glm/gtc/matrix_transform.hpp>

#include "Plane.h"
#include "AABB.h"
#include "Ray.h"
#include "Hittable.h"

Plane::Plane(const glm::vec3& center, const glm::vec3& front, float width, float height, const std::shared_ptr<RrtMaterial>& material)
	         : Hittable(material), m_center(center), 
	           m_front(glm::normalize(front)), m_right(1.f, 0.f, 0.f), m_up(0.f, 1.f, 0.f),
	           m_rotateMat(1.f), m_aabb(new AABB), 
	           m_width(width), m_height(height), m_thickness(0.0001f), m_dataChange(true)
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

		//todo 1. add thickness for plane; 2. revise the hit function, use 3d aabb to measure the hit

		glm::vec3 aa = glm::min(glm::min(glm::min(lb, lt), rb), rt);
		m_aabb->setAA(aa);

		glm::vec3 bb = glm::max(glm::max(glm::max(lb, lt), rb), rt);
		m_aabb->setBB(bb);
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
	float t = (m_center.z - ray.origin().z) / ray.direction().z;
	if (t < tMin || t > tMax)
		return false;

	float x = ray.origin().x + ray.direction().x * t;
	float y = ray.origin().y + ray.direction().y * t;

	if (x < m_aabb->AA().x || x > m_aabb->BB().x || y < m_aabb->AA().y || y > m_aabb->BB().y)
		return false;

	record.hit = true;
	record.u = (x - m_aabb->AA().x) / (m_aabb->BB().x - m_aabb->AA().x);
	record.v = (y - m_aabb->AA().y) / (m_aabb->BB().y - m_aabb->AA().y);
	record.t = t;
	record.pt = ray.at(record.t);

	glm::vec3 outsideN = m_front;
	record.frontFace = glm::dot(ray.direction(), outsideN) < 0.f;
	record.n = record.frontFace ? outsideN : -outsideN;

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

void Plane::setSize(float width, float height)
{
	m_width = width;
	m_height = height;
	m_dataChange.store(true);
}