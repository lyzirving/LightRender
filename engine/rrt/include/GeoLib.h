#ifndef GEO_LIB_H
#define GEO_LIB_H

#include <glm/glm.hpp>

class Ray;

class GeoLib
{
public:
	static glm::vec3 blend(const glm::vec3& start, const glm::vec3& end, float t);
	/* 
	 *check whether ray hits the sphere composed by center and radius.
	 *dist.x contains the smaller time for hitting, dist.y contains the larger time for hitting.
	 *return true, means the ray hits the sphere, false otherwise.
	*/
	static bool hitSphere(const Ray &ray, const glm::vec3 &center, const float radius, glm::vec2 &dist);

private:
	GeoLib() {};
	~GeoLib() = default;

};

#endif
