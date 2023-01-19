#include <cassert>
#include <limits>

#include "GfxThread.h"
#include "GfxContext.h"
#include "SceneRender.h"

#include "SystemUtil.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GfxThread"

GfxThread::GfxThread(const char* name, int32_t fps, RenderType type) 
		   : LightThread(name, true), m_wnd(), m_ctx(new GfxContext), m_renderType(type), m_lastUpdateTime(0),
			 m_lastRecTime(0), m_interval(1000 / fps), m_frameCnt(0), m_fpsCnt(0)
{
	switch (m_renderType)
	{
	case SCENE:
		m_render = std::make_shared<SceneRender>();
		break;
	default:
		break;
	}
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
	if (!m_render)
	{
		LOG_ERR("render is null");
		assert(0);
	}
	m_ctx->bind(m_wnd.hdl_wnd);
	m_render->setViewport(m_wnd.x, m_wnd.y, m_wnd.width, m_wnd.height);
	m_render->init();
}

void GfxThread::onQuit()
{
	m_render->release();
	m_ctx->release();
	m_wnd.hdl_wnd = nullptr;
}

//TODO the way the render thread loops needs to be optimized
void GfxThread::onLoop()
{
	int64_t start = SystemUtil::curTimeMs();

	render();

	int64_t end = SystemUtil::curTimeMs();
	int64_t cost = end - start;
	int64_t wait = m_interval - cost;

	if (wait <= 0)
		return;

	Sleep(wait);

	int64_t nextTick = start + m_interval;
	while (SystemUtil::curTimeMs() < nextTick)
	{
		//do nothing£¬ let the thread runs empty
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
	m_render->update();
	m_ctx->swapBuf();
	recordFps();
}

void GfxThread::recordFps()
{
	m_frameCnt++;
	m_fpsCnt++;
	if (m_frameCnt == INT_MAX)
		m_frameCnt = 0;

	uint64_t cur = GetTickCount64();
	if (m_lastRecTime == 0)
	{
		m_lastRecTime = cur;
	}
	else
	{
		if (cur - m_lastRecTime >= 1000)
		{
			LOG_INFO("fps[%d], total frame[%d]", m_fpsCnt, m_frameCnt);
			m_fpsCnt = 0;
			m_lastRecTime = cur;
		}
	}
}