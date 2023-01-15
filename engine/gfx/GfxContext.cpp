#include <cassert>
#include <GL/glew.h>
#include <GL/wglew.h>

#include "GfxContext.h"
#include "GfxDevice.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxContext"

GfxContext::GfxContext() : m_winInfo()
{
	
}

GfxContext::~GfxContext()
{
	release();
}

void GfxContext::bind(HWND hWnd)
{
	m_winInfo.hWnd = hWnd;
	m_winInfo.hDc = GetDC(hWnd);
	setGLPixFmt();
	m_winInfo.hRc = wglCreateContext(m_winInfo.hDc);
	if (!makeCurrent())
	{
		LOG_ERR("fail to make current when binding");
		assert(0);
	}
	GfxDevice::init();

	//close v-sync, let OpenGL gets the full control
	wglSwapIntervalEXT(false);
}

bool GfxContext::makeCurrent()
{
	return wglMakeCurrent(m_winInfo.hDc, m_winInfo.hRc);
}

void GfxContext::release()
{
	if (m_winInfo.hRc != nullptr)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext(m_winInfo.hRc);
	}
	if (m_winInfo.hDc != nullptr)
	{
		ReleaseDC(m_winInfo.hWnd, m_winInfo.hDc);
	}

	m_winInfo.hDc = nullptr;
	m_winInfo.hRc = nullptr;
}

bool GfxContext::swapBuf()
{
	return SwapBuffers(m_winInfo.hDc);
}

void GfxContext::setGLPixFmt()
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

	m_winInfo.pixFmt = ChoosePixelFormat(m_winInfo.hDc, &pfd);
	if (m_winInfo.pixFmt == 0)
	{
		LOG_ERR("fail to get pixel format");
		assert(0);
	}
	if (!SetPixelFormat(m_winInfo.hDc, m_winInfo.pixFmt, &pfd))
	{
		LOG_ERR("fail to set pixel format");
		assert(0);
	}
}