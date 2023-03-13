#include <cmath>
#include <functional>
#include <glm/gtc/matrix_transform.hpp>

#include "AARectangle.h"
#include "AABB.h"
#include "Ray.h"

#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "AARectangle"

AARectangle::AARectangle(const glm::vec3& center, const glm::vec3& front, const glm::vec3& size, const std::shared_ptr<RrtMaterial>& material)
	         : Hittable(material)
	         , m_center(center)
			 , m_lbFrontPt(0.f), m_lbBackPt(0.f), m_ltFrontPt(0.f), m_ltBackPt(0.f)
	         , m_rbFrontPt(0.f), m_rbBackPt(0.f), m_rtFrontPt(0.f), m_rtBackPt(0.f)
	         , m_front(glm::normalize(front)), m_right(1.f, 0.f, 0.f), m_up(0.f, 1.f, 0.f)
	         , m_aabb(new AABB)
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

		glm::vec3 worldUp{ 0.f, 1.f, 0.f };
		if (m_front == worldUp || m_front == glm::vec3(0.f, -1.f, 0.f))
		{
			m_right = glm::vec3(1.f, 0.f, 0.f);
		}
		else
		{
			m_right = glm::normalize(glm::cross(worldUp, m_front));
		}
		m_up = glm::normalize(glm::cross(m_front, m_right));

		float halfWidth = m_width * 0.5f;
		float halfHeight = m_height * 0.5f;
		float halfThickness = m_thickness * 0.5f;

		glm::vec3 lb = m_center - m_right * halfWidth - m_up * halfHeight;
		glm::vec3 lt = m_center - m_right * halfWidth + m_up * halfHeight;

		glm::vec3 rb = m_center + m_right * halfWidth - m_up * halfHeight;
		glm::vec3 rt = m_center + m_right * halfWidth + m_up * halfHeight;

		m_lbFrontPt = lb + m_front * halfThickness;
		m_lbBackPt = lb - m_front * halfThickness;
		m_ltFrontPt = lt + m_front * halfThickness;
		m_ltBackPt = lt - m_front * halfThickness;

		m_rbFrontPt = rb + m_front * halfThickness;
		m_rbBackPt = rb - m_front * halfThickness;
		m_rtFrontPt = rt + m_front * halfThickness;
		m_rtBackPt = rt - m_front * halfThickness;

		glm::vec3 minBottom = glm::min(glm::min(glm::min(m_lbFrontPt, m_lbBackPt), m_rbFrontPt), m_rbBackPt);
		glm::vec3 minTop = glm::min(glm::min(glm::min(m_ltFrontPt, m_ltBackPt), m_rtFrontPt), m_rtBackPt);

		m_aabb->setAA(glm::min(minBottom, minTop));

		glm::vec3 maxBottom = glm::max(glm::max(glm::max(m_lbFrontPt, m_lbBackPt), m_rbFrontPt), m_rbBackPt);
		glm::vec3 maxTop = glm::max(glm::max(glm::max(m_ltFrontPt, m_ltBackPt), m_rtFrontPt), m_rtBackPt);

		m_aabb->setBB(glm::max(maxBottom, maxTop));
	}
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
	Ray inputRay = ray;
	RrtCompArg arg;
	processAllComp(inputRay, arg);

	glm::vec3 dir = inputRay.direction();
	glm::vec3 invDir = 1.f / dir;

	glm::vec3 AA = m_aabb->AA();
	glm::vec3 BB = m_aabb->BB();

	glm::vec3 far = (BB - inputRay.origin()) * invDir;
	glm::vec3 near = (AA - inputRay.origin()) * invDir;

	glm::vec3 tMax = glm::max(far, near);
	glm::vec3 tMin = glm::min(far, near);

	float t1 = std::min(tMax.x, std::min(tMax.y, tMax.z));
	float t0 = std::max(tMin.x, std::max(tMin.y, tMin.z));

	if (t0 < t1 && t0 > 0.f && t0 > tStart && t0 < tEnd) 
	{
		record.hit = true;
		record.t = t0;
		record.pt = inputRay.at(record.t);
		record.frontFace = true;
		glm::vec3 dstPlaneN;
		if (!hitPtNormal(inputRay.origin(), m_center, record.n))
		{
			record.hit = false;
			record.hitInd = -1;
			return false;
		}
		RrtCompArg arg;
		Ray tmpRay(record.pt, record.n);
		postProcessAllComp(tmpRay, arg);
		record.pt = tmpRay.origin();
		record.n = tmpRay.direction();
		return true;
	}
	else 
	{
		record.hit = false;
		record.hitInd = -1;
		return false;
	}
}

/*
* find the normal of hit plane
* current implementation might be optimized
*/
bool AARectangle::hitPtNormal(const glm::vec3 pt, const glm::vec3 center, glm::vec3& out) const
{
	Ray r;
	r.setOrigin(pt);
	r.setDirection(center - pt);

	glm::vec3 planeCenter, normal;
	float t;

	auto computeT = [&r](glm::vec3 c, glm::vec3 n) 
	{
		return (glm::dot(n, c) - glm::dot(n, r.origin())) / glm::dot(n, r.direction());
	};

	if (glm::dot(r.direction(), m_front) <= 0.f)
	{
		// front plane
		planeCenter = (m_lbFrontPt + m_rtFrontPt) * 0.5f;
		normal = m_front;
		t = computeT(planeCenter, normal);
		if (t >= 0.f)
		{
			out = normal;
			return true;
		}
	} 

	if (glm::dot(r.direction(), -m_front) <= 0.f)
	{
		// back plane
		planeCenter = (m_lbBackPt + m_rtBackPt) * 0.5f;
		normal = -m_front;
		t = computeT(planeCenter, normal);
		if (t >= 0.f)
		{
			out = normal;
			return true;
		}
	}
	
	if (glm::dot(r.direction(), m_right) <= 0.f)
	{
		// right plane
		planeCenter = (m_rbFrontPt + m_rtBackPt) * 0.5f;
		normal = m_right;
		t = computeT(planeCenter, normal);
		if (t >= 0.f)
		{
			out = normal;
			return true;
		}
	}
	
	if (glm::dot(r.direction(), -m_right) <= 0.f)
	{
		// left plane
		planeCenter = (m_lbFrontPt + m_ltBackPt) * 0.5f;
		normal = -m_right;
		t = computeT(planeCenter, normal);
		if (t >= 0.f)
		{
			out = normal;
			return true;
		}
	}
	
	if (glm::dot(r.direction(), m_up) <= 0.f)
	{
		// top plane
		planeCenter = (m_ltFrontPt + m_rtBackPt) * 0.5f;
		normal = m_up;
		t = computeT(planeCenter, normal);
		if (t >= 0.f)
		{
			out = normal;
			return true;
		}
	}
	
	if (glm::dot(r.direction(), -m_up) <= 0.f)
	{
		// bottom plane
		planeCenter = (m_lbFrontPt + m_rbBackPt) * 0.5f;
		normal = -m_up;
		t = computeT(planeCenter, normal);
		if (t >= 0.f)
		{
			out = normal;
			return true;
		}
	}
	return false;
}

glm::vec3 AARectangle::randomPtOnFrontFace()
{
	apply();
	glm::vec3 aa = glm::min(m_lbFrontPt, m_rtFrontPt);
	glm::vec3 bb = glm::max(m_lbFrontPt, m_rtFrontPt);

	glm::vec3 pt;
	pt.x = GfxLib::random(aa.x, bb.x);
	pt.y = GfxLib::random(aa.y, bb.y);
	pt.z = (m_lbFrontPt.z + m_rtFrontPt.z) * 0.5f;
	return pt;
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