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

	static glm::vec3 reflect(const glm::vec3& directionIn, const glm::vec3& n);
	// refract effect obey the Snell's Law where eta1 * sin(theta1) = eta2 * sin(theta2)
	// the refract index in vacuum is 1.0, and other dilectric's refract index is always greater than 1.
	// etaOverEta is eta1 / eta2, eta1 is from the space where the ray come, eta2 belongs to the refracted space
	static glm::vec3 refract(const glm::vec3& directionIn, const glm::vec3& n, float etaOverEta);
	static double reflectance(double cosine, double refractIndex);


private:
	GfxLib() {};
	~GfxLib() = default;

};

#endif