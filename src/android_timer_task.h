#ifndef AY_ANDROID_TIMER_TASK
#define AY_ANDROID_TIMER_TASK

#include "timer_task.h"

class AndroidTimer
    : public Timer
{
protected:
    TimeUnit nanoTime();

public:
    void Update();
};

#endif