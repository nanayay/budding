#ifndef AY_ANDROID_APPLICATION
#define AY_ANDROID_APPLICATION
#include <android_native_app_glue.h>
#include "application.h"
#include "android_task.h"
#include "android_platform.h"
#include "egl_renderer.h"
#include "timer_task.h"
#include "gl_scene.h"

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

    // scene
    Scene* m_pScene;

    bool InitTask();
    bool AddTasks();
};

#endif