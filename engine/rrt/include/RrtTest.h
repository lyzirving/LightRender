#ifndef RRT_TEST_H
#define RRT_TEST_H

#include <glm/glm.hpp>

class RrtCamera;
class Ray;
class Hittable;

class RrtTest
{
public:
	static void main();

private:
	static void draw(const RrtCamera &camera, int width, int height, int channel, uint8_t* data);
	static glm::vec3 rayColor(const Ray &ray, const Hittable &obj);

	RrtTest() {};
	~RrtTest() = default;
};

#endif
