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
#include "LensCamera.h"
#include "PinholeCamera.h"
#include "Ray.h"

#include "Sphere.h"
#include "AARectangle.h"
#include "HittableList.h"
#include "RrtTransform.h"

#include "LambDiffuse.h"
#include "Metal.h"
#include "Dielectric.h"
#include "DiffuseLight.h"

#include "SolidColor.h"

#include "GfxLib.h"

#include "SystemUtil.h"
#include "AssetsMgr.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtTest"

static std::atomic_bool g_running{false};

static std::shared_ptr<AARectangle> g_light{ nullptr };

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

	std::shared_ptr<RrtCamera> camera = std::make_shared<PinholeCamera>();
	camera->setViewportSize(glm::vec2(width, height));
	camera->apply();

	HittableList world;
	glm::vec3 backgroundColor;
	scene2(world, backgroundColor, camera);

	uint8_t* data = (uint8_t *)std::calloc(width * height * channel, sizeof(uint8_t));
	if (!data)
	{
		LOG_ERR("fail to alloc memory");
		assert(0);
	}

	draw(camera, world, backgroundColor, width, height, channel, 10, 9, data);

	int ret = stbi_write_png(path.c_str(), width, height, channel, data, 0);
	if (ret == 0)
	{
		LOG_ERR("fail to write png[%s]", path.c_str());
	}

	std::free(data);

	if (g_light)
		g_light.reset();

	int64_t end = SystemUtil::curTimeMs();
	LOG_INFO("finish generating rrt img[%s], take time[%.3f]", path.c_str(), (end - start) / 1000.f);

	g_running.store(false);
}

void RrtTest::draw(const std::shared_ptr<RrtCamera>& camera, const HittableList& obj, const glm::vec3& backgroundColor, 
	               const int width, const int height, const int channel,
	               const int sampleCnt, const int reflectCnt, uint8_t* data)
{
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
				u = float(col) + GfxLib::random();
				v = float(row) + GfxLib::random();
				Ray ray = camera->getRay(u, v);
				glm::vec3 ret;
				// hard coded to use gradient background for invalid input background color
				if (backgroundColor.x < 0.f || backgroundColor.y < 0.f || backgroundColor.z < 0.f)
					ret = rayColor(ray, obj, reflectCnt);
				else
					ret = rayColor(ray, obj, reflectCnt, backgroundColor);
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

		std::shared_ptr<RrtMaterial> mat = objList.at(rec.hitInd)->getMatl();

		glm::vec3 emitVal = mat->emit(rec.u, rec.v, rec.pt);

		float pdf{1.f};

		if (!mat->scatter(ray, rec, attenu, scatterRay, pdf))
			return emitVal;

		float scatterPdf = mat->scatterPdf(ray, rec, scatterRay);

		glm::vec3 retColor = rayColor(scatterRay, objList, --reflectDepth);
		return emitVal + attenu * scatterPdf * retColor / pdf;
	}
	else
	{
		glm::vec3 startColor{ 1.f }, endColor{ 0.5f, 0.7f, 1.f };
		float t = (ray.direction().y + 1.f) * 0.5f;
		return GfxLib::blend(startColor, endColor, t);
	}
}

glm::vec3 RrtTest::rayColor(const Ray& ray, const HittableList& objList, int reflectDepth, const glm::vec3& background)
{
	if (reflectDepth <= 0) return glm::vec3(0.f);

	HitRecord rec;
	// use 0.001 instead of 0.f to fix shadow acne
	if (objList.hit(ray, 0.001f, FLT_MAX, rec) && rec.hitInd >= 0)
	{
		Ray scatterRay;
		glm::vec3 attenu{ 1.f };

		std::shared_ptr<RrtMaterial> mat = objList.at(rec.hitInd)->getMatl();

		glm::vec3 emitVal = mat->emit(rec.u, rec.v, rec.pt);

		float pdf{ 1.f };

		if (!mat->scatter(ray, rec, attenu, scatterRay, pdf))
			return emitVal;

		float scatterPdf = mat->scatterPdf(ray, rec, scatterRay);

		glm::vec3 retColor = rayColor(scatterRay, objList, --reflectDepth, background);
		return emitVal + attenu * scatterPdf * retColor / pdf;
	}
	else
	{
		return background;
	}
}

void RrtTest::scene0(HittableList& world, glm::vec3& backgroundColor, const std::shared_ptr<RrtCamera>& camera)
{
	std::shared_ptr<Hittable> centerSphere = std::make_shared<Sphere>(glm::vec3(0.f, 0.f, 0.f), 0.5f, 
		                                                              std::make_shared<LambDiffuse>(glm::vec3(0.7f, 0.3f, 0.3f)));

	std::shared_ptr<Hittable> leftSphere = std::make_shared<Sphere>(glm::vec3(-1.f, 0.f, 0.f), 0.5f, 
		                                                            std::make_shared<Metal>(glm::vec3(0.8f, 0.8f, 0.8f)));

	std::shared_ptr<Hittable> rightSphere = std::make_shared<Sphere>(glm::vec3(1.f, 0.f, 0.f), 0.5f,
		                                                             std::make_shared<Metal>(glm::vec3(0.8f, 0.6f, 0.2f), 0.5f));

	// todo: fix the dilectric material's bug when total internal reflection is enabled
	/*std::shared_ptr<Hittable> sphereRight = std::make_shared<Sphere>(glm::vec3(1.f, 0.f, 0.f), 0.5f);
	std::shared_ptr<RrtMaterial> rightMatl = std::make_shared<Dilectric>(1.5f);
	sphereRight->setMatl(rightMatl);*/

	std::shared_ptr<Hittable> groundSphere = std::make_shared<Sphere>(glm::vec3(0.f, -100.5f, 0.f), 100.f,
		                                                              std::make_shared<LambDiffuse>(glm::vec3(0.8f, 0.8f, 0.f)));
	world.add(centerSphere);
	world.add(leftSphere);
	world.add(rightSphere);
	world.add(groundSphere);

	// invalid color
	backgroundColor = glm::vec3(-1.f);
}

void RrtTest::scene1(HittableList &world, glm::vec3& backgroundColor, const std::shared_ptr<RrtCamera>& camera)
{
	//scene1 is a scene with light

	std::shared_ptr<Hittable> centerSphere = std::make_shared<Sphere>(glm::vec3(0.f, 0.f, 0.f), 0.5f, 
		                                                              std::make_shared<LambDiffuse>(glm::vec3(0.7f, 0.3f, 0.3f)));
	std::shared_ptr<Hittable> groundSphere = std::make_shared<Sphere>(glm::vec3(0.f, -100.5f, 0.f), 100.f,
		                                                              std::make_shared<LambDiffuse>(glm::vec3(0.8f, 0.8f, 0.f)));
	g_light = std::make_shared<AARectangle>(glm::vec3(1.f, 0.f, 0.f), glm::vec3(-1.f, 0.f, 0.f), glm::vec3(0.8f, 1.f, 0.1f),
		                                                               std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(glm::vec3(1.f))));

	world.add(centerSphere);
	world.add(groundSphere);
	world.add(g_light);

	backgroundColor = glm::vec3(0.f);
}

void RrtTest::scene2(HittableList& world, glm::vec3& backgroundColor, const std::shared_ptr<RrtCamera> &camera)
{
	//scene2 is cornel box
	std::shared_ptr<AARectangle> left = std::make_shared<AARectangle>(
		glm::vec3(-1.f, 0.f, 0.f), 
		glm::vec3(1.f, 0.f, 0.f), 
		glm::vec3(2.f, 2.f, 0.01f),
		std::make_shared<LambDiffuse>(glm::vec3(0.12f, 0.45f, 0.15f)));

	std::shared_ptr<AARectangle> right = std::make_shared<AARectangle>(
		glm::vec3(1.f, 0.f, 0.f),
		glm::vec3(-1.f, 0.f, 0.f),
		glm::vec3(2.f, 2.f, 0.01f),
		std::make_shared<LambDiffuse>(glm::vec3(0.65f, 0.05f, 0.05f)));

	std::shared_ptr<AARectangle> top = std::make_shared<AARectangle>(
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f, -1.f, 0.f),
		glm::vec3(2.f, 2.f, 0.01f),
		std::make_shared<LambDiffuse>(glm::vec3(0.73f, 0.73f, 0.73f)));

	std::shared_ptr<AARectangle> bottom = std::make_shared<AARectangle>(
		glm::vec3(0.f, -1.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(2.f, 2.f, 0.01f),
		std::make_shared<LambDiffuse>(glm::vec3(0.73f, 0.73f, 0.73f)));

	std::shared_ptr<AARectangle> back = std::make_shared<AARectangle>(
		glm::vec3(0.f, 0.f, -1.f),
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(2.f, 2.f, 0.01f),
		std::make_shared<LambDiffuse>(glm::vec3(0.73f, 0.73f, 0.73f)));

	g_light = std::make_shared<AARectangle>(
		glm::vec3(0.f, 1.f - 0.01 * 0.5f, 0.f),
		glm::vec3(0.f, -1.f, 0.f),
		glm::vec3(0.7f, 0.7f, 0.1f),
		std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(glm::vec3(3.f))));

	std::shared_ptr<AARectangle> rectangle = std::make_shared<AARectangle>(
		glm::vec3(-0.4f, -1.f + 0.01f + 0.6f, -0.1f),
		glm::vec3(0.f, 0.f, 1.f),
		glm::vec3(0.5f, 1.2f, 0.6f),
		std::make_shared<LambDiffuse>(glm::vec3(0.73f, 0.73f, 0.73f)));

	std::shared_ptr<RrtTransform> trans = std::make_shared<RrtTransform>();
	trans->setTranslation(glm::vec3(0.25f, 0.f, 0.f));
	trans->setRotate(20.f, glm::vec3(0.f, 1.f, 0.f));
	rectangle->addComp(RRT_TRANSFORM, trans);

	world.add(left);
	world.add(right);
	world.add(top);
	world.add(bottom);
	world.add(back);
	world.add(g_light);
	world.add(rectangle);

	backgroundColor = glm::vec3(0.f);

	camera->setPosition(glm::vec3(0.f, 0.f, 2.3f));
	camera->apply();
}