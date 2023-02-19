#ifndef RRT_CAMERA_H
#define RRT_CAMERA_H

#include <glm/glm.hpp>

#include "Ray.h"

class RrtCamera
{
public:
	RrtCamera();
	virtual ~RrtCamera();

	Ray getRay(float u, float v) const;

	void apply();

	void setApertureLen(float len);
	void setFocalLen(float focal);
	void setFov(float fov);
	void setLookAt(const glm::vec3& at);
	void setPosition(const glm::vec3& pos);
	void setViewportSize(const glm::vec2& size);

private:
	void calcCamMat();
	void calcPrjMat();
	void calcScreenMat();
	void calcInvMat();

	float m_focalLen;
	float m_fov;
	float m_apertureLen;
	// x is width, y is height
	glm::vec2 m_viewportSize;

	glm::vec3 m_camPos;
	glm::vec3 m_lookAt;
	glm::vec3 m_camFront, m_camRight, m_camUp;

	glm::mat4 m_camMat, m_prjMat, m_screenMat;
	glm::mat4 m_invMat, m_camInv, m_screenPrjInv;
};

#endif
