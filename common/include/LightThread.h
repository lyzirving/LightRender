#ifndef LIGHT_THREAD_H
#define LIGHT_THREAD_H

#include <thread>
#include <string>
#include <atomic>

class LightThread
{
public:
    LightThread(const char* name, bool loop = true);

    virtual ~LightThread();
    virtual void onFirst() = 0;
    virtual void onQuit() = 0;
    virtual void onLoop() = 0;

    void interrupt();
    bool isInterrupted();
    void join();
    void start();

protected:
    void run();

    std::thread m_thread;

    std::string m_name;
    std::atomic<bool> m_interrupted;
    std::atomic<bool> m_isRunning;
    bool m_loop;

};


#endif // !LIGHT_THREAD_H

