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
	inline const glm::vec3 &position() const { return m_pos; }

	Ray getRay(float u, float v) const;

	void setAspect(float aspect);
	void setFocalLen(float focal);
	void setPosition(const glm::vec3& pos);

private:
	void computeImgLb();

	float m_aspect;
	float m_focalLen;

	// camera's position in world coordinate system.
	glm::vec3 m_pos;

	// image's size is measured by virtual unit.
	float m_imgHeight, m_imgWidth;

	// a left-bottom corner(-m_imgWidth / 2.f, -m_imgHeight / 2.f) of img 
	// the pt is measured in camera coordinate system which is a standard right-handed coordinate system
	glm::vec3 m_imgLb;
};

#endif
