#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "AARectangle.h"
#include "AABB.h"
#include "Ray.h"
#include "Hittable.h"

AARectangle::AARectangle(const glm::vec3& center, const glm::vec3& front, const glm::vec3& size, const std::shared_ptr<RrtMaterial>& material)
	         : Hittable(material)
	         , m_center(center)
	         , m_front(glm::normalize(front)), m_right(1.f, 0.f, 0.f), m_up(0.f, 1.f, 0.f)
	         , m_rotateMat(1.f), m_aabb(new AABB)
	         , m_width(size.x), m_height(size.y), m_thickness(size.z)
	         , m_dataChange(true)
{
	apply();
}

AARectangle::~AARectangle()
{
	m_aabb.reset();
}

void AARectangle::apply()
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
		glm::vec3 lbBack = lb - m_front * halfThickness;
		glm::vec3 ltFront = lt + m_front * halfThickness;
		glm::vec3 ltBack = lt - m_front * halfThickness;

		glm::vec3 rbFront = rb + m_front * halfThickness;
		glm::vec3 rbBack = rb - m_front * halfThickness;
		glm::vec3 rtFront = rt + m_front * halfThickness;
		glm::vec3 rtBack = rt - m_front * halfThickness;

		glm::vec3 minBottom = glm::min(glm::min(glm::min(lbFront, lbBack), rbFront), rbBack);
		glm::vec3 minTop = glm::min(glm::min(glm::min(ltFront, ltBack), rtFront), rtBack);

		m_aabb->setAA(glm::min(minBottom, minTop));

		glm::vec3 maxBottom = glm::max(glm::max(glm::max(lbFront, lbBack), rbFront), rbBack);
		glm::vec3 maxTop = glm::max(glm::max(glm::max(ltFront, ltBack), rtFront), rtBack);

		m_aabb->setBB(glm::max(maxBottom, maxTop));
	}
}

void AARectangle::rotate(float angle, const glm::vec3& axis)
{
	glm::mat4 I{ 1.f };
	I = glm::rotate(I, glm::radians(angle), axis);
	m_rotateMat = I * m_rotateMat;
	m_dataChange.store(true);
}

void AARectangle::resetRotate()
{
	m_rotateMat = glm::mat4(1.f);
	m_dataChange.store(true);
}

bool AARectangle::boundingBox(AABB& box) const
{
	box = *m_aabb;
	return true;
}

glm::vec3 AARectangle::center() const
{
	return m_center;
}

bool AARectangle::hit(const Ray& ray, float tStart, float tEnd, HitRecord& record) const
{
	glm::vec3 dir = ray.direction();
	glm::vec3 invDir = 1.f / dir;

	glm::vec3 AA = m_aabb->AA();
	glm::vec3 BB = m_aabb->BB();

	glm::vec3 far = (BB - ray.origin()) * invDir;
	glm::vec3 near = (AA - ray.origin()) * invDir;

	glm::vec3 tMax = glm::max(far, near);
	glm::vec3 tMin = glm::min(far, near);

	float t1 = std::min(tMax.x, std::min(tMax.y, tMax.z));
	float t0 = std::max(tMin.x, std::max(tMin.y, tMin.z));

	if (t0 < t1 && t0 > 0.f && t0 > tStart && t0 < tEnd) 
	{
		record.hit = true;
		record.t = t0;
		record.pt = ray.at(record.t);
		record.frontFace = true;
		// t0 is the nearest pt that hits the surface of rectangle£¬ it should always be the outside normal
		record.n = glm::normalize(record.pt - m_center);
		return true;
	}
	else 
	{
		record.hit = false;
		record.hitInd = -1;
		return false;
	}
}

void AARectangle::setCenter(const glm::vec3& center)
{
	m_center = center;
	m_dataChange.store(true);
}

void AARectangle::setFront(const glm::vec3& front)
{
	m_front = glm::normalize(front);
	m_dataChange.store(true);
}

void AARectangle::setSize(float width, float height, float thickness)
{
	m_width = width;
	m_height = height;
	m_thickness = thickness;
	m_dataChange.store(true);
}