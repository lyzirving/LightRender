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

#include "RrtTest.h"
#include "RrtCamera.h"
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

	RrtCamera camera;
	camera.setAspect(float(width) / float(height));

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
}

void RrtTest::draw(const RrtCamera& camera, int width, int height, int channel, uint8_t* data)
{
	glm::vec3 startColor{ 1.f }, endColor{0.5f, 0.7f, 1.f};

	Sphere sphere(glm::vec3(0.f, 0.f, -2.f), 0.5f);

	int outputRow, outputCol;
	float u, v;
	Ray ray;
	HitRecord rec;

	// row and col start from left-bottom(-imgWidth / 2.f, -imgHeight / 2.f) corner for rendering
	// outputRow and outputCol start from left-top(0, 0) corner for image output
	for (int row = 0; row < height; ++row)
	{
		LOG_INFO("scanlines remaining: %d", height - 1 - row);
		for (int col = 0; col < width; ++col)
		{
			outputCol = col;
			outputRow = height - 1 - row;

			u = float(col) / float(width - 1);
			v = float(row) / float(height - 1);

			ray = camera.getRay(u, v);

			sphere.hit(ray, 0.f, FLT_MAX, rec);

			if (rec.hit)
			{
				data[(outputRow * width + outputCol) * channel + 0] = static_cast<uint8_t>((rec.n.x + 1.f) * 0.5f * 255.999);
				data[(outputRow * width + outputCol) * channel + 1] = static_cast<uint8_t>((rec.n.y + 1.f) * 0.5f * 255.999);
				data[(outputRow * width + outputCol) * channel + 2] = static_cast<uint8_t>((rec.n.z + 1.f) * 0.5f * 255.999);
			}
			else
			{
				glm::vec3 rayDir = ray.direction();
				float t = (rayDir.y + 1.f) * 0.5f;
				glm::vec3 val = GeoLib::blend(startColor, endColor, t);

				data[(outputRow * width + outputCol) * channel + 0] = static_cast<uint8_t>((val.x) * 255.999);
				data[(outputRow * width + outputCol) * channel + 1] = static_cast<uint8_t>((val.y) * 255.999);
				data[(outputRow * width + outputCol) * channel + 2] = static_cast<uint8_t>((val.z) * 255.999);
			}
		}
	}
}