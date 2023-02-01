#ifndef RRT_CAMERA_H
#define RRT_CAMERA_H

#include <glm/glm.hpp>

class RrtCamera
{
public:
	RrtCamera();
	virtual ~RrtCamera();

	inline float aspect() const { return m_aspect; }
	inline float focal() const { return m_focalLen; }
	inline float imgWidth() const { return m_imgWidth; }
	inline float imgHeight() const { return m_imgHeight; }
	inline const glm::vec3 &position() const { return m_pos; }
	inline const glm::vec3& imgLt() const { return m_imgLt; }

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

	// a left-top corner of img in camera coordinate system which is a standard right-handed coordinate system
	glm::vec3 m_imgLt;
};

#endif
