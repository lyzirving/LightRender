#include <cmath>
#include <cstdlib>

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

float GeoLib::clamp(float val, float min, float max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    return val;
}

glm::vec3 GeoLib::gamma2Correct(const glm::vec3& color)
{
    glm::vec3 ret;
    ret.x = std::sqrt(color.x);
    ret.y = std::sqrt(color.y);
    ret.z = std::sqrt(color.z);
    return ret;
}

double GeoLib::random()
{
    // returns a random real in [0, 1)
    return std::rand() / (RAND_MAX + 1.f);
}

double GeoLib::random(double min, double max)
{
    return min + (max - min) * random();
}

glm::vec3 GeoLib::randomVec3(double min, double max)
{
    return glm::vec3(random(min, max), random(min, max), random(min, max));
}

glm::vec3 GeoLib::randomInUnitSphere()
{
    while (true)
    {
        glm::vec3 ret = randomVec3(-1.f, 1.f);
        if (glm::length(ret) >= 1.f) continue;
        return ret;
    }
}