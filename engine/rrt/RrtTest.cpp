#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <algorithm>
#include <cassert>

#include "RrtTest.h"

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

	uint8_t* data = (uint8_t *)std::calloc(width * height * channel, sizeof(uint8_t));
	if (!data)
	{
		LOG_ERR("fail to alloc memory");
		assert(0);
	}

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

	int ret = stbi_write_png(path.c_str(), width, height, channel, data, 0);
	if (ret == 0)
	{
		LOG_ERR("fail to write png[%s]", path.c_str());
	}

	std::free(data);

	int64_t end = SystemUtil::curTimeMs();
	LOG_INFO("finish generating rrt img[%s], take time[%.3f]", path.c_str(), (end - start) / 1000.f);
}