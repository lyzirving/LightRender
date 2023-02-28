#include "Sphere.h"
#include "Ray.h"
#include "LambDiffuse.h"
#include "Metal.h"
#include "AABB.h"

#include "GfxLib.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Sphere"

Sphere::Sphere() : Hittable(), m_center(0.f), m_radius(0.f) 
{
    m_matl = std::make_shared<LambDiffuse>();
}

Sphere::Sphere(const glm::vec3& center, const float radius) : Hittable(), m_center(center), m_radius(radius)
{
    m_matl = std::make_shared<LambDiffuse>();
}

Sphere::Sphere(const glm::vec3& center, const float radius, const std::shared_ptr<RrtMaterial>& material) 
       : Hittable(material), m_center(center), m_radius(radius)
{
}

Sphere::~Sphere() = default;

bool Sphere::boundingBox(AABB& box) const
{
    box.setAA(glm::vec3(m_center.x - m_radius, m_center.y - m_radius, m_center.z - m_radius));
    box.setBB(glm::vec3(m_center.x + m_radius, m_center.y + m_radius, m_center.z + m_radius));
    return true;
}

glm::vec3 Sphere::center() const 
{
    return m_center;
}

bool Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    glm::vec3 oc = ray.origin() - m_center;
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2.f * glm::dot(oc, ray.direction());
    float c = glm::dot(oc, oc) - std::pow(m_radius, 2);
    float discriminant = std::pow(b, 2) - 4 * a * c;
    if (discriminant < 0.f)
    {
        record.hit = false;
        record.hitInd = -1;
        return false;
    }
    
    // select the smaller result at first
    float root = (-b - std::sqrt(discriminant)) / (2.f * a);
    if (root < tMin || root > tMax)
    {
        root = (-b + std::sqrt(discriminant)) / (2.f * a);
        if (root < tMin || root > tMax)
        {
            record.hit = false;
            record.hitInd = -1;
            return false;
        }
    }

    record.hit = true;
    record.t = root;
    record.pt = ray.at(record.t);
    glm::vec3 outsideN = (record.pt - m_center) / m_radius;
    record.frontFace = glm::dot(ray.direction(), outsideN) < 0.f;
    record.n = record.frontFace ? outsideN : -outsideN;
    return true;
}