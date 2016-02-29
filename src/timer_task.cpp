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
