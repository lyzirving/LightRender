#include <functional>

#include "LightThread.h"

#include "Logger.h"
#ifdef LOCAL_TAG
#undef LOCAL_TAG
#endif
#define LOCAL_TAG "LightThread"

LightThread::LightThread(const char* name, bool loop) : m_thread(), m_name(name),
                                                        m_interrupted(false), m_isRunning(false), 
                                                        m_loop(loop)
{
}

LightThread::~LightThread()
{
    if (!m_interrupted.load())
    {
        m_interrupted.store(true);
    }

    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void LightThread::interrupt()
{
    m_interrupted.store(true);
}

bool LightThread::isInterrupted()
{
    return m_interrupted.load();
}

void LightThread::join()
{
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void LightThread::run()
{
    m_isRunning.store(true);
    LOG_INFO("thread[%s] start", m_name.c_str());
    if (m_loop)
    {
        onFirst();
        while (!m_interrupted.load())
        {
            onLoop();
        }
    }
    else
    {
        onLoop();
    }
    m_isRunning.store(false);
    onQuit();
    LOG_INFO("thread[%s] quit", m_name.c_str())
}

void LightThread::start()
{
    if (!m_isRunning.load())
    {
        std::thread thread(std::bind(&LightThread::run, this));
        m_thread = std::move(thread);
    }
    else
    {
        LOG_INFO("thread[%s] is already running", m_name.c_str());
    }
}