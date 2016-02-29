#ifndef AY_TIMER_TASK
#define AY_TIMER_TASK
#include "task.h"
#include "type_defines.h"

class Timer
    :public Task
{
protected:
    TimeUnit m_lastFrameTime;
    float m_deltaFrameTime;
    float m_deltaMultiplier;
    float m_delta;

protected:
    // 1 sec = 1000 million sec
    // 1 million sec = 1000 micro sec
    // 1 micro sec = 1000 nano sec
    virtual TimeUnit nanoTime() = 0;

public:
    explicit Timer(unsigned int priority = Task::Priority::High);
    virtual ~Timer();

    bool Start();
    void OnSuspend();
    void OnResume();
    void Stop();

    float getFrameTimeDelta() const
    {
        return this->m_delta;
    };

    float setFrameTimeMultiplier(const float deltaMultiplier)
    {
       this->m_deltaMultiplier = deltaMultiplier; 
    };
};

#endif