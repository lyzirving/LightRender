#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <memory>
#include <atomic>

#include "RrtTest.h"
#include "RrtCamera.h"
#include "Ray.h"

#include "Sphere.h"
#include "HittableList.h"

#include "LambDiffuse.h"
#include "Metal.h"
#include "Dielectric.h"

#include "GfxLib.h"

#include "SystemUtil.h"
#include "AssetsMgr.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtTest"

static std::atomic_bool g_running{false};

bool RrtTest::checkRunning() { return g_running.load(); }

void RrtTest::main()
{
	g_running.store(true);

	int64_t start = SystemUtil::curTimeMs();

	const int width = 720;
	const int height = 540;
	const int channel = 3;
	std::string path = AssetsMgr::getRootDir();
	const char oldVal = '\\';
	const char newVal = '/';
	std::replace(path.begin(), path.end(), oldVal, newVal);
	path.append("/RrtTest.png");

	RrtCamera camera;
	camera.setPosition(glm::vec3(-2.f, 2.f, 3.f));
	camera.setLookAt(glm::vec3(0.f, 0.f, -1.f));
	camera.setAspect(float(width) / float(height));
	camera.setFov(45);

	uint8_t* data = (uint8_t *)std::calloc(width * height * channel, sizeof(uint8_t));
	if (!data)
	{
		LOG_ERR("fail to alloc memory");
		assert(0);
	}

	draw(camera, width, height, channel, data);

	int ret = stbi_write_png(path.c_str(), width, height, channel, data, 0);
	if (ret == 0)
	{
		LOG_ERR("fail to write png[%s]", path.c_str());
	}

	std::free(data);

	int64_t end = SystemUtil::curTimeMs();
	LOG_INFO("finish generating rrt img[%s], take time[%.3f]", path.c_str(), (end - start) / 1000.f);

	g_running.store(false);
}

void RrtTest::draw(const RrtCamera& camera, int width, int height, int channel, uint8_t* data)
{
	HittableList world;
	std::shared_ptr<Hittable> sphereCenter = std::make_shared<Sphere>(glm::vec3(0.f, 0.f, -1.f), 0.5f);
	std::shared_ptr<Matl> centerMatl = std::make_shared<LambDiffuse>(glm::vec3(0.7f, 0.3f, 0.3f));
	sphereCenter->setMatl(centerMatl);

	std::shared_ptr<Hittable> sphereLeft = std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, -1.f), 0.5f);
	std::shared_ptr<Matl> leftMatl = std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f));
	sphereLeft->setMatl(leftMatl);

	std::shared_ptr<Hittable> sphereRight = std::make_shared<Sphere>(glm::vec3(1.f, 0.f, -1.f), 0.5f);
	std::shared_ptr<Matl> rightMatl = std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f));
	static_cast<Metal*>(rightMatl.get())->setFluzzy(0.5f);
	sphereRight->setMatl(rightMatl);

	/*std::shared_ptr<Hittable> sphereRight = std::make_shared<Sphere>(glm::vec3(1.f, 0.f, -2.f), 0.5f);
	std::shared_ptr<Matl> rightMatl = std::make_shared<Dilectric>(1.5f);
	sphereRight->setMatl(rightMatl);*/

	std::shared_ptr<Hittable> sphereGround = std::make_shared<Sphere>(glm::vec3(0.f, -100.5f, -1.f), 100.f);
	std::shared_ptr<Matl> groundMatl = std::make_shared<LambDiffuse>(glm::vec3(0.8f, 0.8f, 0.f));
	sphereGround->setMatl(groundMatl);

	world.add(sphereCenter);
	world.add(sphereLeft);
	world.add(sphereRight);
	world.add(sphereGround);

	const int sampleCnt = 10;
	const int reflectCnt = 5;

	int outputRow, outputCol;
	float u, v;

	// row and col start from left-bottom(-imgWidth / 2.f, -imgHeight / 2.f) corner for rendering
	// outputRow and outputCol start from left-top(0, 0) corner for image output
	for (int row = 0; row < height; ++row)
	{
		LOG_INFO("scanlines remaining: %d", (height - 1 - row));
		for (int col = 0; col < width; ++col)
		{
			glm::vec3 sampleColor{0.f};
			float scale = 1.f / float(sampleCnt);

			// create a number of samples to form an average color of a pixel to perform antialiasing
			for (int i = 0; i < sampleCnt; ++i)
			{
				u = (col + GfxLib::random()) / float(width - 1);
				v = (row + GfxLib::random()) / float(height - 1);
				Ray ray = camera.getRay(u, v);
				glm::vec3 ret = rayColor(ray, world, reflectCnt);
				sampleColor = sampleColor + ret * scale;
			}

			sampleColor = GfxLib::gamma2Correct(sampleColor);

			outputCol = col;
			outputRow = height - 1 - row;
			data[(outputRow * width + outputCol) * channel + 0] = static_cast<uint8_t>(256 * GfxLib::clamp(sampleColor.x, 0.f, 0.999f));
			data[(outputRow * width + outputCol) * channel + 1] = static_cast<uint8_t>(256 * GfxLib::clamp(sampleColor.y, 0.f, 0.999f));
			data[(outputRow * width + outputCol) * channel + 2] = static_cast<uint8_t>(256 * GfxLib::clamp(sampleColor.z, 0.f, 0.999f));
		}
	}
}

glm::vec3 RrtTest::rayColor(const Ray& ray, const HittableList& objList, int reflectDepth)
{
	if (reflectDepth <= 0) return glm::vec3(0.f);

	HitRecord rec;
	// use 0.001 instead of 0.f to fix shadow acne
	if (objList.hit(ray, 0.001f, FLT_MAX, rec) && rec.hitInd >= 0)
	{
		Ray scatterRay;
		glm::vec3 attenu{ 1.f };

		std::shared_ptr<Matl> mat = objList.at(rec.hitInd)->getMatl();
		if (!mat->scatter(ray, rec, attenu, scatterRay))
			return glm::vec3(0.f);

		glm::vec3 retColor = rayColor(scatterRay, objList, --reflectDepth);
		return attenu * retColor;
	}
	else
	{
		glm::vec3 startColor{ 1.f }, endColor{ 0.5f, 0.7f, 1.f };
		float t = (ray.direction().y + 1.f) * 0.5f;
		return GfxLib::blend(startColor, endColor, t);
	}
}