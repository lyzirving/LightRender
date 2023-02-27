#ifndef RRT_SOLID_COLOR_H
#define RRT_SOLID_COLOR_H

#include "RrtTexture.h"

class SolidColor : public RrtTexture
{
public:
	SolidColor();
	SolidColor(const glm::vec3 &color);
	virtual ~SolidColor();

	virtual glm::vec3 value(float u, float v, const glm::vec3& p) const override;

private:
	glm::vec3 m_color;
};

#endif