#include "SolidColor.h"

SolidColor::SolidColor() : RrtTexture(), m_color(0.f)
{
}

SolidColor::SolidColor(const glm::vec3& color) : RrtTexture(), m_color(color)
{
}

SolidColor::~SolidColor() = default;
 
glm::vec3 SolidColor::value(float u, float v, const glm::vec3& p) const
{
	return m_color;
}