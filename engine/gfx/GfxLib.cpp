#include <cmath>
#include <cstdlib>

#include "GfxLib.h"

#include "ViewLib.h"

#include "Ray.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxLib"

float GfxLib::PI = std::acos(-1.f);

glm::vec3 GfxLib::blend(const glm::vec3& start, const glm::vec3& end, float t)
{
    if (t < 0.f)
        t = 0.f;
    else if (t > 1.f)
        t = 1.f;

    return (1 - t) * start + t * end;
}

float GfxLib::clamp(float val, float min, float max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    return val;
}

glm::vec3 GfxLib::gamma2Correct(const glm::vec3& color)
{
    glm::vec3 ret;
    ret.x = std::sqrt(color.x);
    ret.y = std::sqrt(color.y);
    ret.z = std::sqrt(color.z);
    return ret;
}

bool GfxLib::nearZero(const glm::vec3& input)
{
    const double s = 1e-6;
    return (std::abs(input.x) < s && std::abs(input.y) < s && std::abs(input.z) < s);
}

bool GfxLib::nearZero(float input)
{
    const float s = 1e-6;
    return std::abs(input) < s;
}

double GfxLib::random()
{
    // returns a random real in [0, 1)
    return std::rand() / (RAND_MAX + 1.f);
}

double GfxLib::random(double min, double max)
{
    return min + (max - min) * random();
}

int GfxLib::randomInt(int min, int max)
{
    return static_cast<int>(random(double(min), double(max + 1)));
}

glm::vec3 GfxLib::randomVec3(double min, double max)
{
    return glm::vec3(random(min, max), random(min, max), random(min, max));
}

glm::vec3 GfxLib::randomInUnitSphere()
{
    float theta = random(0.f, 180.f);
    float phi = random(0.f, 360.f);
    float radius = random();

    glm::vec3 ret;
    ViewLib::SCStoCCS(theta, phi, radius, ret);

    return ret;

    /*while (true)
    {
        glm::vec3 ret = randomVec3(-1.f, 1.f);
        if (glm::length(ret) >= 1.f) continue;
        return ret;
    }*/
}

glm::vec3 GfxLib::randomInHemisphere(const glm::vec3 n)
{
    glm::vec3 res = randomInUnitSphere();
    res = (glm::dot(n, res) >= 0.f) ? res : (-1.f * res);
    return res;
}

glm::vec3 GfxLib::randomOnUnitSphere()
{
    float theta = random(0.f, 180.f);
    float phi = random(0.f, 360.f);

    glm::vec3 ret;
    ViewLib::SCStoCCS(theta, phi, 1.f, ret);

    return ret;
}

glm::vec3 GfxLib::reflect(const glm::vec3& directionIn, const glm::vec3& n)
{
    float b = glm::dot(directionIn, n);
    glm::vec3 refDir = directionIn - 2 * b * n;
    return glm::normalize(refDir);
}

glm::vec3 GfxLib::refract(const glm::vec3& directionIn, const glm::vec3& n, float etaOverEta)
{
    float cosTheta = std::fmin(glm::dot(-directionIn, n), 1.f);
    glm::vec3 rayPerp = etaOverEta * (directionIn + cosTheta * n);
    glm::vec3 rayParal = -(float)std::sqrt(std::fabs(1.f - glm::length(rayPerp))) * n;
    return glm::normalize(rayPerp + rayParal);
}

double GfxLib::reflectance(double cosine, double refractIndex)
{
    // Schlick's approximation for reflectance
    double r0 = (1.0 - refractIndex) / (1.0 + refractIndex);
    r0 = std::pow(r0, 2);
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}