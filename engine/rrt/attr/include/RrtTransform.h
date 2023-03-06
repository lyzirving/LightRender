#ifndef RRT_TRANSFORM_H
#define RRT_TRANSFORM_H

#include <glm/glm.hpp>

#include "RrtComponent.h"

class RrtTransform : public RrtComponent
{
public:
	RrtTransform();
	virtual ~RrtTransform();

	virtual void process(Ray& ray, RrtCompArg& arg) override;
	virtual void postProcess(Ray& ray, RrtCompArg& arg) override;

	inline void setTranslation(const glm::vec3& trans) { m_translation = trans; }
	inline void setRotate(float angle, const glm::vec3& axis) 
	{ 
		m_rotateAngle = angle;
		m_rotateAxis = axis;
	}

private:
	glm::vec3 m_translation;
	glm::vec3 m_rotateAxis;
	float m_rotateAngle;
};

#endif