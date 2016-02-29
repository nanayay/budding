#ifdef _GDB_DEBUG_
    #include <unistd.h>
#endif

#include "android_application.h"
#include "log.h"
#include "kernel.h"

AndroidApplication::AndroidApplication(android_app* state)
    : m_pState(state),
      m_pRenderer(NULL),
      m_pTimer(NULL),
      m_pAndroidTask(NULL)
{
}

AndroidApplication::~AndroidApplication()
{
}

bool AndroidApplication::Initialize()
{
    LOGD("app_dummy");
    app_dummy();

#ifdef _GDB_DEBUG_
    LOGD("Begin wait 10sec for gdb debug");
    sleep(10);
    LOGD("End wait 10sec for gdb debug");
#endif

    bool result = false;

    result = InitKernel();

    result = InitTask();
    result = AddTasks();

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

bool AndroidApplication::InitKernel()
{
    LOGD("android application init kernel begin");
    Kernel* kernel = new Kernel();
    // todo, check is this good style to write this kind of codes
    LOGD("android application init kernel end");

    LOGD("android application kernel's pointer is %x", reinterpret_cast<int>(Kernel::GetSingletonPtr()));

    return true;
}

bool AndroidApplication::InitTask()
{
    bool result = true;

    // Note, Application will new tasks, but not delete them, the AndroidTask will inform Kernel to delete them
    // Note, Application will feed the tasks, but not manage them or control them, it is the AndroidTask [message handler] and Kernel's job

    LOGD("Android Timer create");
    m_pTimer = new AndroidTimer();

    LOGD("EGLRenderer create");
    m_pRenderer = new EGLRenderer(m_pState);

    LOGD("Android Task create");
    m_pAndroidTask = new AndroidTask(m_pState, m_pRenderer);

    return result;
}

bool AndroidApplication::AddTasks()
{
    bool result = true;

    LOGD("Android Timer add to kernel");
    Kernel::GetSingleton().AddTask(m_pTimer);

    LOGD("EGLRenderer add to kernel");
    Kernel::GetSingleton().AddTask(m_pRenderer);

    LOGD("Android Task add to kernel");
    Kernel::GetSingleton().AddTask(m_pAndroidTask);
 
    return result;
}


