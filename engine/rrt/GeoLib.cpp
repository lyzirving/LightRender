#include <cmath>

#include "GeoLib.h"
#include "Ray.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GeoLib"

glm::vec3 GeoLib::blend(const glm::vec3& start, const glm::vec3& end, float t)
{
    if (t < 0.f)
        t = 0.f;
    else if (t > 1.f)
        t = 1.f;

    return (1 - t) * start + t * end;
}

bool GeoLib::hitSphere(const Ray& ray, const glm::vec3 &center, const float radius, glm::vec2& dist)
{
    glm::vec3 oc = ray.origin() - center;
    float a = glm::dot(ray.direction(), ray.direction());
    float b = 2.f * glm::dot(oc, ray.direction());
    float c = glm::dot(oc, oc) - std::pow(radius, 2);
    float discriminant = std::pow(b, 2) - 4 * a * c;
    if (discriminant < 0.f)
    {
        return false;
    }
    else
    {
        dist.x = (-b - std::sqrt(discriminant)) / (2.f * a);
        dist.y = (-b + std::sqrt(discriminant)) / (2.f * a);
        return true;
    }
}