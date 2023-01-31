#include <cmath>
#include <cassert>

#include "ViewLib.h"
#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "ViewLib"

glm::vec3 ViewLib::blend(const glm::vec3& start, const glm::vec3& end, float t)
{
    if (t < 0.f)
        t = 0.f;
    else if (t > 1.f)
        t = 1.f;

    return (1 - t) * start + t * end;
}

void ViewLib::SCStoCCS(float theta, float phi, float r, glm::vec3& out)
{
    float projection = std::abs(r * std::sin(glm::radians(theta)));
    out.x = projection * std::sin(glm::radians(phi));
    out.y = r * std::cos(glm::radians(theta));
    out.z = projection * std::cos(glm::radians(phi));
}

void ViewLib::CCStoSCS(const glm::vec3& ccs, glm::vec3& out)
{
    float len = std::sqrt(std::pow(ccs.x, 2) + std::pow(ccs.y, 2) + std::pow(ccs.z, 2));
    out.x = glm::degrees(std::acos(ccs.y / len));
    out.z = len;

    // tan(a+b) = (tan(a) + tan(b)) / (1 - tan(a)tan(b))
    // tan(a-b) = (tan(a) - tan(b)) / (1 + tan(a)tan(b))
    if (ccs.z == 0.f)
    {
        if (ccs.x == 0.f)
        {
            Logger::err("err input for ccs, x and z can not be 0.f at the same time");
            assert(0);
        }

        if (ccs.x > 0.f)
            out.y = 90.f;
        else
            out.y = 270.f;
    }
    else
    {
        float tanTheta{ 0.f };
        if (ccs.x >= 0.f && ccs.z > 0.f)
        {
            tanTheta = ccs.x / ccs.z;
            out.y = glm::degrees(std::atan(tanTheta));
        }
        else if (ccs.x >= 0.f && ccs.z < 0.f)
        {
            tanTheta = ccs.x / std::abs(ccs.z);
            out.y = 180.f - glm::degrees(std::atan(tanTheta));
        }
        else if (ccs.x <= 0.f && ccs.z < 0.f)
        {
            tanTheta = ccs.x / ccs.z;
            out.y = 180.f + glm::degrees(std::atan(tanTheta));
        }
        else if (ccs.x <= 0.f && ccs.z > 0.f)
        {
            tanTheta = std::abs(ccs.x) / ccs.z;
            out.y = 360.f - glm::degrees(std::atan(tanTheta));
        }
    }
}

