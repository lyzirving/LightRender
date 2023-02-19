#ifndef RRT_TEST_H
#define RRT_TEST_H

#include <glm/glm.hpp>

class RrtCamera;
class Ray;
class HittableList;

class RrtTest
{
public:
	static void main();
	static bool checkRunning();

private:
	static void draw(const RrtCamera &camera, const HittableList& obj, const int width, const int height, const int channel,
		             const int sampleCnt, const int reflectCnt, uint8_t* data);
	static glm::vec3 rayColor(const Ray& ray, const HittableList& obj, int reflectDepth);

	RrtTest() {};
	~RrtTest() = default;
};

#endif
