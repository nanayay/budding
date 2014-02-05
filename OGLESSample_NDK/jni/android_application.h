#ifndef AY_ANDROID_APPLICATION
#define AY_ANDROID_APPLICATION
#include "application.h"
#include "log.h"
#include "kernel.h"
#include "android_task.h"
#include "android_platform.h"
#include "egl_renderer.h"

class AndroidApplication
    : public Application
{
public:
    AndroidApplication(android_app* state);
    virtual ~AndroidApplication();

    virtual bool Initialize();
    virtual void Run();

private:
    android_app* m_pState;

    bool AddTasks();
};

#endif