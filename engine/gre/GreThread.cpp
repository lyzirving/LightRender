#include <cassert>
#include <limits>

#include "GreThread.h"
#include "GreEventQueue.h"
#include "GreContext.h"

#include "GfxShaderMgr.h"

#include "SceneRender.h"
#include "RrtRender.h"

#include "SystemUtil.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "GreThread"

GreThread::GreThread(const char* name, int32_t fps, RenderType type) 
		   : LightThread(name, true), m_wnd(), m_ctx(new GreContext), m_renderType(type), m_lastUpdateTime(0),
			 m_lastRecTime(0), m_interval(1000 / fps), m_frameCnt(0), m_fpsCnt(0), 
	         m_uiEvtQueue(new GreEventQueue(GreEventType::EVT_TYPE_OUTSIDE))
{
	switch (m_renderType)
	{
	case SCENE:
		m_render = std::make_shared<SceneRender>();
		break;
	case RRT_CANVAS:
		m_render = std::make_shared<RrtRender>();
		break;
	default:
		break;
	}
}

GreThread::~GreThread()
{
	m_uiEvtQueue.reset();
	m_wnd.hdl_wnd = nullptr;
	m_ctx.reset();
}

void GreThread::dealEvent()
{
	if (m_uiEvtQueue && !m_uiEvtQueue->empty())
	{
		GreEvent evt = m_uiEvtQueue->dequeue();
		processUi(evt);
	}
}

void GreThread::onFirst()
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
	m_ctx->initInterface();
	GfxShaderMgr::get()->init();

	m_render->setViewport(m_wnd.x, m_wnd.y, m_wnd.width, m_wnd.height);
	m_render->init();
}

void GreThread::onQuit()
{
	// watch out the release order
	m_render->release();
	GfxShaderMgr::release();
	m_ctx->release();
	m_wnd.hdl_wnd = nullptr;
}

//TODO the way the render thread loops needs to be optimized
void GreThread::onLoop()
{
	int64_t start = SystemUtil::curTimeMs();

	dealEvent();

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

void GreThread::onWindowSizeChange(int width, int height)
{
	if (m_uiEvtQueue)
	{
		GreEvent evt = { GreEventId::EVT_WINDOW_SIZE_CHANGE, width, height };
		m_uiEvtQueue->enqueue(std::move(evt));
	}
}

void GreThread::processUi(GreEvent& evt)
{
	switch (evt.m_id)
	{
	case GreEventId::EVT_WINDOW_SIZE_CHANGE:
	{
		m_wnd.width = evt.intArg0;
		m_wnd.height = evt.intArg1;
		m_render->setViewport(m_wnd.x, m_wnd.y, m_wnd.width, m_wnd.height);
		break;
	}
	default:
		break;
	}
}

void GreThread::setWindowInfo(HWND wnd, int x, int y, int width, int height)
{
	m_wnd.hdl_wnd = wnd;
	m_wnd.x = x;
	m_wnd.y = y;
	m_wnd.width = width;
	m_wnd.height = height;
}

void GreThread::render()
{
	m_render->update();
	m_ctx->swapBuf();
	recordFps();
}

void GreThread::recordFps()
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