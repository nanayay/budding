#include "android_timer_task.h"

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
