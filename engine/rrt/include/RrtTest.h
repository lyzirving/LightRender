#ifndef RRT_TEST_H
#define RRT_TEST_H

#include <glm/glm.hpp>
#include <memory>

class RrtCamera;
class Ray;
class HittableList;
class AARectangle;
struct HitRecord;

class RrtTest
{
public:
	static void main();
	static bool checkRunning();

private:
	static void draw(const std::shared_ptr<RrtCamera> &camera, const HittableList& obj, const glm::vec3& backgroundColor,
		             const int width, const int height, const int channel,
		             const int sampleCnt, const int reflectCnt, uint8_t* data);
	static glm::vec3 rayColor(const Ray& ray, const HittableList& obj, int reflectDepth);
	static glm::vec3 rayColor(const Ray& ray, const HittableList& obj, int reflectDepth, const glm::vec3 &background);
	static bool sampleToLight(const std::shared_ptr<AARectangle>& lightArea, const HitRecord &rec, Ray &scatter, float &pdf);
	static void scene0(HittableList &world, glm::vec3& backgroundColor, const std::shared_ptr<RrtCamera> &camera);
	static void scene1(HittableList &world, glm::vec3& backgroundColor, const std::shared_ptr<RrtCamera> &camera);
	static void scene2(HittableList& world, glm::vec3& backgroundColor, const std::shared_ptr<RrtCamera> &camera);

	RrtTest() {};
	~RrtTest() = default;
};

#endif
