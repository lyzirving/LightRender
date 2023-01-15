#include <mutex>
#include <cassert>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GfxDevice.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxDevice"

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
		LOG_ERR("fail to init glew");
		assert(0);
	}
	GLint major{ 0 }, minor{0};
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	LOG_INFO("gl version, major[%d], minor[%d]", major, minor);
}

void GfxDevice::setViewport(int l, int b, int width, int height)
{
	glViewport(l, b, width, height);
}

void GfxDevice::update() 
{
	glClearColor(1.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}