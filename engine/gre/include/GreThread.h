#ifndef GRE_THREAD_H
#define GRE_THREAD_H

#include <memory>
#include <Windows.h>

#include "LightThread.h"
#include "ViewDef.h"

class GreContext;
class Render;

class GreEventQueue;
struct GreEvent;

class GreThread : public LightThread
{
public:

    GreThread(const char* name, int32_t fps, RenderType type);
    
    virtual ~GreThread();
    virtual void onFirst() override;
    virtual void onQuit() override;
    virtual void onLoop() override;

    void onWindowSizeChange(int width, int height);
    void setWindowInfo(HWND wnd, int x, int y, int width, int height);

protected:
    struct WindowInfo
    {
        HWND hdl_wnd;
        int x, y;
        int width, height;
        WindowInfo() : hdl_wnd(nullptr), x(0), y(0), width(0), height(0) {}
    };

    void dealEvent();
    void processUi(GreEvent &evt);
    void render();
    void recordFps();

    WindowInfo m_wnd;
    std::shared_ptr<GreContext> m_ctx;
    std::shared_ptr<Render> m_render;
    RenderType m_renderType;

    uint64_t m_lastUpdateTime, m_lastRecTime;
    int64_t m_interval;
    int32_t m_frameCnt, m_fpsCnt;

    std::shared_ptr<GreEventQueue> m_uiEvtQueue;
};

#endif // !GRE_THREAD_H

