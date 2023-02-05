#ifndef GFX_LIB_H
#define GFX_LIB_H

#include <glm/glm.hpp>

class Ray;

class GfxLib
{
public:
	static glm::vec3 blend(const glm::vec3& start, const glm::vec3& end, float t);
	static float clamp(float val, float min, float max);

	// divide the color by gamma-correct for gamma = 2.0.
	static glm::vec3 gamma2Correct(const glm::vec3& color);

	static bool nearZero(const glm::vec3 &input);

	static double random();
	static double random(double min, double max);

	static glm::vec3 randomVec3(double min, double max);
	static glm::vec3 randomInUnitSphere();
	static glm::vec3 randomOnUnitSphere();


private:
	GfxLib() {};
	~GfxLib() = default;

};

#endif