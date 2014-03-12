#include "android_application.h"
#include "log.h"
#include "kernel.h"

AndroidApplication::AndroidApplication(android_app* state)
    : m_pState(state),
      m_pRenderer(NULL),
      m_pTimer(NULL),
      m_pAndroidTask(NULL),
      m_pScene(NULL)
{
}

AndroidApplication::~AndroidApplication()
{
    // TODO, make some way to manage Scene, not call delete scene here
    // TODO, use MyInit() is not such good way, just use Init(), that is clear for sub class's read
    // TODO, add code to check the pointer before delete
    if (m_pScene)
    {
        m_pScene->Destroy();
        delete m_pScene;
    }
}

bool AndroidApplication::Initialize()
{
    LOGD("app_dummy");
    app_dummy();

    bool result = false;

    result = InitTask();
    result = AddTasks();

    return result;
}

void AndroidApplication::Run()
{
    LOGD("kernel execute begin");
    Kernel::getSingleton().Execute();
    LOGD("kernel execute end");

    LOGD("android clear closing begin");
    AndroidTask::ClearClosing();
    LOGD("android clear closing end");
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

    // Feed the EGLRenderer with Renderables after all Tasks have been created

    // TODO Here
    // SceneManager<GLScene>, scene tree, current scene, new scene, delete scene, load scene from outside files, delete them when goto other scene like menu, game scene, 
    // Scene->GLScene->Menu Scene -> Game Scene
    // GLBasicScene->Build(EGLRenderer)
    //                  -> new GLRenderable vector
    //                  -> assign to EGLRenderer

    m_pScene = new GLBasicScene();
    m_pScene->Init();
    m_pScene->MakeCurrent(m_pRenderer);

    //TODO Here, find some place to delete m_pScene, other than delete them in application's destructor

    return result;
}

bool AndroidApplication::AddTasks()
{
    bool result = true;

    LOGD("Android Timer add to kernel");
    Kernel::getSingleton().AddTask(m_pTimer);

    LOGD("EGLRenderer add to kernel");
    Kernel::getSingleton().AddTask(m_pRenderer);

    LOGD("Android Task add to kernel");
    Kernel::getSingleton().AddTask(m_pAndroidTask);
 
    return result;
}


