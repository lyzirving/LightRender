#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

#include "RrtTransform.h"
#include "Ray.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtTransform"

RrtTransform::RrtTransform() : RrtComponent()
                             , m_translation(0.f)
                             , m_rotateAxis(1.f, 0.f, 0.f)
                             , m_rotateAngle(0.f)
{
}

RrtTransform::~RrtTransform() = default;

void RrtTransform::process(Ray& ray, RrtCompArg& arg)
{
	glm::vec3 origin = ray.origin();
	glm::vec3 dir = ray.direction();

	glm::mat4 mat{ 1.f };

	mat = glm::translate(mat, -m_translation);
	mat = glm::rotate(mat, glm::radians(-m_rotateAngle), m_rotateAxis);

	// excute rotation first, then translation
	origin = mat * glm::vec4(origin, 1.f);
	dir = mat * glm::vec4(dir, 0.f);

	ray.setOrigin(origin);
	ray.setDirection(dir);
}

void RrtTransform::postProcess(Ray& ray, RrtCompArg& arg)
{
	glm::vec3 origin = ray.origin();
	glm::vec3 dir = ray.direction();

	glm::mat4 mat{ 1.f };

	mat = glm::translate(mat, m_translation);
	mat = glm::rotate(mat, glm::radians(m_rotateAngle), m_rotateAxis);

	// excute rotation first, then translation
	origin = mat * glm::vec4(origin, 1.f);
	dir = mat * glm::vec4(dir, 0.f);

	ray.setOrigin(origin);
	ray.setDirection(dir);
}