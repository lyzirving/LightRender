#include <mutex>
#include <cassert>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GfxDevice.h"
#include "Logger.h"

static std::mutex g_mutex{};
static GfxDevice* g_device{ nullptr };

GfxDevice* GfxDevice::get()
{
	if (!g_device)
	{
		std::lock_guard<std::mutex> lock(g_mutex);
		if (!g_device)
		{
			g_device = new GfxDevice;
		}
	}
	return g_device;
}

void GfxDevice::init()
{
	if (glewInit() != GLEW_OK)
	{
		Logger::err("fail to init glew");
		assert(0);
	}
}