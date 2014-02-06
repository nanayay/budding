#include <time.h>
#include "timer_task.h"

Timer::Timer(unsigned int priority)
    : Task(priority),
      m_lastFrameTime(0),
      m_deltaMultiplier(1),
      m_delta(0)
{
}

Timer::~Timer() {}

bool Timer::Start()
{
    this->m_lastFrameTime = nanoTime();
    return true;
}

void Timer::OnSuspend() {}

void Timer::OnResume()
{
    this->m_lastFrameTime = nanoTime();
}

void Timer::Stop() {}

TimeUnit AndroidTimer::nanoTime()
{
    timespec now;
    int err = clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec * 1000000000L + now.tv_nsec;
}

void AndroidTimer::Update()
{
    // get the delta between the last frame and current moment
    TimeUnit now = nanoTime();
    const float _MULIPLIER_ = 0.000000001f;
    float deltaFrameTime = (now - m_lastFrameTime) * _MULIPLIER_;
    m_lastFrameTime = now;
    m_delta = deltaFrameTime * m_deltaMultiplier;
}
