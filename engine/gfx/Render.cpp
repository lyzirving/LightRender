#include <GL/glew.h>
#include <GL/wglew.h>
#include <cassert>

#include "Render.h"
#include "ViewTransform.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "Render"

Render::Render() : m_viewport(), m_transform(new ViewTransform) {}

Render::~Render() 
{
	m_transform.reset();
}

void Render::init()
{
	if (glewInit() != GLEW_OK)
	{
		LOG_ERR("fail to init glew");
		assert(0);
	}
	GLint major{ 0 }, minor{ 0 };
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	LOG_INFO("gl version, major[%d], minor[%d]", major, minor);
	//close v-sync, let OpenGL gets the full control
	wglSwapIntervalEXT(false);
}

void Render::update() 
{
	preRender();
	onRender();
	postRender();
}

void Render::setViewport(int l, int b, int width, int height)
{
	m_viewport.x = l;
	m_viewport.y = b;
	m_viewport.z = width;
	m_viewport.w = height;
}

void Render::preRender() 
{
	glViewport(m_viewport.x, m_viewport.y, m_viewport.z, m_viewport.w);
	glClearColor(0.f, 0.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::onRender() {}

void Render::postRender() {}

void Render::release() {}