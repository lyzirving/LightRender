#include <cassert>

#include "GfxThread.h"
#include "GfxContext.h"
#include "GfxDevice.h"

#include "SystemUtil.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxThread"

GfxThread::GfxThread(const char* name, int32_t fps) : LightThread(name, true), m_wnd(), m_ctx(new GfxContext), m_interval(1000 / fps),
                                                      m_lastTimeMs(0)
{
}

GfxThread::~GfxThread()
{
	m_wnd.hdl_wnd = nullptr;
	m_ctx.reset();
}

void GfxThread::onFirst()
{
	if (!m_wnd.hdl_wnd)
	{
		LOG_ERR("window handle is null");
		assert(0);
	}
	m_ctx->bind(m_wnd.hdl_wnd);
	GfxDevice::setViewport(m_wnd.x, m_wnd.y, m_wnd.width, m_wnd.height);
}

void GfxThread::onQuit()
{
	m_ctx->release();
	m_wnd.hdl_wnd = nullptr;
}

void GfxThread::onLoop()
{
	int64_t cur = SystemUtil::curTimeMs();

	//TODO use microseconds to control frame rate
	if (m_lastTimeMs == 0 || (m_lastTimeMs + m_interval) >= cur)
	{
		//first time enter or the time exceeds the limit, do work direcly
		m_lastTimeMs = cur;
		render();
	}
	else 
	{
		// sleep and do work
		Sleep(cur - (m_lastTimeMs + m_interval));
		m_lastTimeMs = SystemUtil::curTimeMs();
		render();
	}
}

void GfxThread::setWindowInfo(HWND wnd, int x, int y, int width, int height)
{
	m_wnd.hdl_wnd = wnd;
	m_wnd.x = x;
	m_wnd.y = y;
	m_wnd.width = width;
	m_wnd.height = height;
}

void GfxThread::render()
{
	GfxDevice::get()->update();
	m_ctx->swapBuf();
}