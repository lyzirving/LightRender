#ifndef RRT_CAMERA_H
#define RRT_CAMERA_H

#include <glm/glm.hpp>

#include "Ray.h"

class RrtCamera
{
public:
	RrtCamera();
	virtual ~RrtCamera();

	inline float aspect() const { return m_aspect; }
	inline float focal() const { return m_focalLen; }
	inline const glm::vec3 &position() const { return m_camPos; }

	Ray getRay(float u, float v) const;

	void setAspect(float aspect);
	void setFocalLen(float focal);
	void setFov(float fov);
	void setLookAt(const glm::vec3& at);
	void setPosition(const glm::vec3& pos);

private:
	void computeAxis();
	void computeAttr();
	void computeLb();

	float m_aspect;
	float m_focalLen;
	float m_fov;

	float m_viewportWidth, m_viewportHeight;

	// left-bottom corner(center.x - m_viewportWidth / 2.f, center.y - m_viewportHeight / 2.f, -flocal length) of viewport
	// viewport's +x axis points to horizontally right, +y axis points to vertically up, +z is inverse direction of lookAt
	// the pt is measured in camera coordinate system which is a standard right-handed coordinate system
	glm::vec3 m_viewportLb;

	glm::vec3 m_camPos;
	glm::vec3 m_lookAt;
	glm::vec3 m_camFront, m_camRight, m_camUp;
};

#endif
