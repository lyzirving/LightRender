#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <algorithm>
#include <cassert>
#include <limits>

#include "RrtTest.h"
#include "Ray.h"
#include "Sphere.h"
#include "GeoLib.h"

#include "SystemUtil.h"
#include "AssetsMgr.h"

#include "Logger.h"

#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "RrtTest"

void RrtTest::gradientImg(int width, int height, int channel, uint8_t* data)
{
	for (int row = 0; row < height; ++row)
	{
		LOG_INFO("scanlines remaining: %d", height - row);
		for (int col = 0; col < width; ++col)
		{
			data[(row * width + col) * channel + 0] = static_cast<uint8_t>(float(col) / (width - 1) * 255.999f);
			data[(row * width + col) * channel + 1] = static_cast<uint8_t>(float(height - 1 - row) / (height - 1) * 255.999f);
			data[(row * width + col) * channel + 2] = static_cast<uint8_t>(0.25f * 255.999f);
		}
	}
}

void RrtTest::main()
{
	int64_t start = SystemUtil::curTimeMs();

	const int width = 720;
	const int height = 540;
	const int channel = 3;
	std::string path = AssetsMgr::getRootDir();
	const char oldVal = '\\';
	const char newVal = '/';
	std::replace(path.begin(), path.end(), oldVal, newVal);
	path.append("/RrtTest.png");

	uint8_t* data = (uint8_t *)std::calloc(width * height * channel, sizeof(uint8_t));
	if (!data)
	{
		LOG_ERR("fail to alloc memory");
		assert(0);
	}

	draw(width, height, channel, data);

	int ret = stbi_write_png(path.c_str(), width, height, channel, data, 0);
	if (ret == 0)
	{
		LOG_ERR("fail to write png[%s]", path.c_str());
	}

	std::free(data);

	int64_t end = SystemUtil::curTimeMs();
	LOG_INFO("finish generating rrt img[%s], take time[%.3f]", path.c_str(), (end - start) / 1000.f);
}

void RrtTest::draw(int width, int height, int channel, uint8_t* data)
{
	float aspect = float(width) / float(height);
	float viewportHeight = 2.f;
	float viewportWidth = viewportHeight * aspect;

	glm::vec3 start{ 1.f }, end{0.5f, 0.7f, 1.f};

	glm::vec3 cameraPos{ 0.f }, dir{ 0.f };
	float focalLen = 1.f;
	glm::vec3 leftTop{-viewportWidth / 2.f, -viewportHeight / 2.f, -focalLen};

	Ray ray;
	ray.setOrigin(cameraPos);

	Sphere sphere(glm::vec3(0.f, 0.f, -2.f), 0.5f);
	HitRecord rec;

	for (int row = 0; row < height; ++row)
	{
		LOG_INFO("scanlines remaining: %d", height - row);
		for (int col = 0; col < width; ++col)
		{
			dir = leftTop;
			dir += glm::vec3(float(col) / float(width - 1) * viewportWidth, 0.f, 0.f);
			dir += glm::vec3(0.f, float(row) / float(height - 1) * viewportHeight, 0.f);
			dir = glm::normalize(dir - ray.origin());
			ray.setDirection(dir);

			sphere.hit(ray, 0.f, FLT_MAX, rec);
			if (rec.hit)
			{
				data[(row * width + col) * channel + 0] = 255;
				data[(row * width + col) * channel + 1] = 0;
				data[(row * width + col) * channel + 2] = 0;
			}
			else
			{
				glm::vec3 rayDir = ray.direction();
				// keep t in [0, 1.f]
				float t = (rayDir.y + 1.f) * 0.5f;
				glm::vec3 val = GeoLib::blend(start, end, t);

				data[(row * width + col) * channel + 0] = static_cast<uint8_t>((val.x) * 255.999);
				data[(row * width + col) * channel + 1] = static_cast<uint8_t>((val.y) * 255.999);
				data[(row * width + col) * channel + 2] = static_cast<uint8_t>((val.z) * 255.999);
			}
		}
	}
}