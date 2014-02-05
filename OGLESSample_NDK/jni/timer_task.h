#ifndef AY_TIMER_TASK
#define AY_TIMER_TASK
#include "task.h"

class Timer
    :public Task
{
public:
    Timer(unsigned int priority);
    virtual ~Timer();

    /* data */
};

#endif