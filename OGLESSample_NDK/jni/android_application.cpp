#include "android_application.h"


AndroidApplication::AndroidApplication(android_app* state)
    : m_pState(state)
{
}

AndroidApplication::~AndroidApplication()
{
}

bool AndroidApplication::Initialize()
{
    LOGD("app_dummy");
    app_dummy();

    bool result = AddTasks();

    return result;
}

void AndroidApplication::Run()
{
    LOGD("kernel execute begin");
    Kernel::GetSingleton().Execute();
    LOGD("kernel execute end");

    LOGD("android clear closing begin");
    AndroidTask::ClearClosing();
    LOGD("android clear closing end");
}

bool AndroidApplication::AddTasks()
{
    LOGD("EGLRenderer create and add to kernel begin");
    Renderer* pEGLRenderer = new EGLRenderer(m_pState);
    Kernel::GetSingleton().AddTask(pEGLRenderer);
    LOGD("EGLRenderer create and add to kernel end");

    LOGD("android task create and add to kernel begin");
    AndroidTask* pAndroidTask = new AndroidTask(m_pState, pEGLRenderer);
    Kernel::GetSingleton().AddTask(pAndroidTask);
    LOGD("android task create and add to kernel end");

    return true;
}


