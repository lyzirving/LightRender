#ifndef RRT_CAMERA_H
#define RRT_CAMERA_H

#include <glm/glm.hpp>
#include <atomic>

#include "Ray.h"

class RrtCamera
{
public:
	RrtCamera();
	virtual ~RrtCamera();

	virtual Ray getRay(float u, float v) = 0;

	inline const glm::vec3 front() { return m_camFront; }
	inline const glm::vec3 right() { return m_camRight; }
	inline const glm::vec3 up() { return m_camUp; }

	void apply();
	void setLookAt(const glm::vec3& at);
	void setNearFar(float near, float far);
	void setPosition(const glm::vec3& pos);
	void setViewportSize(const glm::vec2& size);

protected:
	void calcCamMat();
	void calcPrjMat();
	void calcScreenMat();
	void calcInvMat();

	float m_fov;
	float m_near, m_far;

	glm::vec3 m_camPos;
	glm::vec3 m_lookAt;
	glm::vec3 m_camFront, m_camRight, m_camUp;
	glm::vec2 m_viewportSize;

	glm::mat4 m_camMat, m_prjMat, m_screenMat;
	glm::mat4 m_invMat, m_camInv, m_screenPrjInv;

	std::atomic<bool> m_dataChange;
};

#endif