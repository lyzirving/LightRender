#include <cassert>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GreContext.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreContext"

GreContext::GreContext() : m_winInfo()
{
	
}

GreContext::~GreContext()
{
	release();
}

void GreContext::bind(HWND wnd)
{
	m_winInfo.hdl_wnd = wnd;
	m_winInfo.hdl_dc = GetDC(wnd);
	setGLPixFmt();
	m_winInfo.hdl_rc = wglCreateContext(m_winInfo.hdl_dc);
	if (!makeCurrent())
	{
		LOG_ERR("fail to make current when binding");
		assert(0);
	}
}

void GreContext::initInterface()
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

bool GreContext::makeCurrent()
{
	return wglMakeCurrent(m_winInfo.hdl_dc, m_winInfo.hdl_rc);
}

void GreContext::release()
{
	if (m_winInfo.hdl_rc != nullptr)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_winInfo.hdl_rc);
	}
	if (m_winInfo.hdl_dc != nullptr)
	{
		ReleaseDC(m_winInfo.hdl_wnd, m_winInfo.hdl_dc);
	}

	m_winInfo.hdl_dc = nullptr;
	m_winInfo.hdl_rc = nullptr;
}

bool GreContext::swapBuf()
{
	return SwapBuffers(m_winInfo.hdl_dc);
}

void GreContext::setGLPixFmt()
{
	PIXELFORMATDESCRIPTOR pfd{};
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	m_winInfo.pixFmt = ChoosePixelFormat(m_winInfo.hdl_dc, &pfd);
	if (m_winInfo.pixFmt == 0)
	{
		LOG_ERR("fail to get pixel format");
		assert(0);
	}
	if (!SetPixelFormat(m_winInfo.hdl_dc, m_winInfo.pixFmt, &pfd))
	{
		LOG_ERR("fail to set pixel format");
		assert(0);
	}
}