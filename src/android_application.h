#ifndef AY_ANDROID_APPLICATION
#define AY_ANDROID_APPLICATION
#include <android_native_app_glue.h>
#include "application.h"
#include "android_platform.h"
#include "android_task.h"
#include "egl_renderer.h"
#include "timer_task.h"

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

    // tasks
    EGLRenderer* m_pRenderer;
    AndroidTimer* m_pTimer;
    AndroidTask* m_pAndroidTask;

    bool InitKernel();
    bool InitTask();
    bool AddTasks();
};

#endif