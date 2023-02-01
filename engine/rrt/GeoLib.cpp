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

double GeoLib::random()
{
    // returns a random real in [0, 1)
    return std::rand() / (RAND_MAX + 1.f);
}

double GeoLib::random(double min, double max)
{
    return min + (max - min) * random();
}