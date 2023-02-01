#include "Sphere.h"
#include "Ray.h"

Sphere::Sphere() : Hittable(), m_center(0.f), m_radius(0.f) {}

Sphere::Sphere(const glm::vec3& center, const float radius) : Hittable(), m_center(center), m_radius(radius) {}

Sphere::~Sphere() = default;

void Sphere::hit(const Ray& ray, float tMin, float tMax, HitRecord& record) const
{
    glm::vec3 oc = ray.origin() - m_center;
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2.f * glm::dot(oc, ray.direction());
    float c = glm::dot(oc, oc) - std::pow(m_radius, 2);
    float discriminant = std::pow(b, 2) - 4 * a * c;
    if (discriminant < 0.f)
    {
        record.hit = false;
        return;
    }
    
    // select the smaller result at first
    float root = (-b - std::sqrt(discriminant)) / (2.f * a);
    if (root < tMin || root > tMax)
    {
        root = (-b + std::sqrt(discriminant)) / (2.f * a);
        if (root < tMin || root > tMax)
        {
            record.hit = false;
            return;
        }
    }

    record.hit = true;
    record.t = root;
    record.pt = ray.at(root);
    // the normal always points outward from the sphere 
    record.n = glm::normalize(record.pt - m_center);
}