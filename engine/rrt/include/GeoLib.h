#ifndef GEO_LIB_H
#define GEO_LIB_H

#include <glm/glm.hpp>

class Ray;

class GeoLib
{
public:
	static glm::vec3 blend(const glm::vec3& start, const glm::vec3& end, float t);
	static double random();
	static double random(double min, double max);

private:
	GeoLib() {};
	~GeoLib() = default;

};

#endif
