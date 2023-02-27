#ifndef RRT_TEXTURE_H
#define RRT_TEXTURE_H

#include <glm/glm.hpp>

class RrtTexture
{
public:
	RrtTexture() {};
	virtual ~RrtTexture() = default;

	virtual glm::vec3 value(float u, float v, const glm::vec3 &p) const = 0;
};

#endif
