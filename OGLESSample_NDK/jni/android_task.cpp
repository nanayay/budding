#include "kernel.h"
#include "android_task.h"
#include "log.h"
#include "gl_scene.h"
#include "android_asset.h"

static int32_t android_handle_input(struct android_app* app,
		AInputEvent* event) {
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {
		size_t pointerCount = AMotionEvent_getPointerCount(event);
		for (size_t i = 0; i < pointerCount; ++i) {
			LOGI("Received motion event from pointer %zu: (%.2f, %.2f)", i,
					AMotionEvent_getX(event, i), AMotionEvent_getY(event, i));
		}
		return 1;
	} else if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_KEY) {
		LOGI("Received key event: %d", AKeyEvent_getKeyCode(event));
		return 1;
	}
	return 0;
}

static void android_handle_cmd(struct android_app* app, int cmd) {
	switch (cmd) {
	case APP_CMD_SAVE_STATE: {
		// The system has asked us to save our current state. Do so
		LOGD("android_handle_cmd() APP_CMD_SAVE_STATE cmd begin");
		LOGD("android_handle_cmd() APP_CMD_SAVE_STATE cmd end");
		}
		break;
	case APP_CMD_INIT_WINDOW: {
		// get the window ready for showing
		LOGD("android_handle_cmd() APP_CMD_INIT_WINDOW cmd begin");
		if (app->window != NULL)
		{
			LOGD("android_handle_cmd() app->window != null");
            Engine* pEngine = (Engine*)app->userData;
			pEngine->getRenderer()->Init();
            pEngine->getScene()->Load();
            pEngine->getRenderer()->Bind(pEngine->getScene());
		}
		else
		{
			LOGD("android_handle_cmd() app->window == null");
		}
		LOGD("android_handle_cmd() APP_CMD_INIT_WINDOW cmd end");
		}
		break;
	case APP_CMD_DESTROY: {
		//
		LOGD("android_handle_cmd() APP_CMD_DESTROY cmd begin");
		Engine* pEngine = (Engine*)app->userData;
        pEngine->getRenderer()->Bind(NULL);
        pEngine->getScene()->UnLoad();
		pEngine->getRenderer()->Destroy();
		LOGD("android_handle_cmd() APP_CMD_DESTROY cmd end");
		// TODO, inform kernel to kill all task?
		}
		break;
	case APP_CMD_TERM_WINDOW: {
		// clean up the window because it is being hidden/closed
		LOGD("android_handle_cmd() APP_CMD_TERM_WINDOW cmd begin");
        Engine* pEngine = (Engine*)app->userData;
        pEngine->getRenderer()->Bind(NULL);
        pEngine->getScene()->UnLoad();
        pEngine->getRenderer()->Destroy();
        LOGD("android_handle_cmd() APP_CMD_TERM_WINDOW cmd end");
		}
		break;
	case APP_CMD_GAINED_FOCUS: {
		// when our app gains forcus, start some senser, like start monitoring the accelerometer, open camera, etc.
		LOGD("android_handle_cmd() APP_CMD_GAINED_FOCUS cmd begin");
		LOGD("android_handle_cmd() APP_CMD_GAINED_FOCUS cmd end");
		}
		break;
	case APP_CMD_LOST_FOCUS: {
		// when our app loses focus, we stop some senser, like stop monitoring the accelerometer
		// this is to avoid consuming battery while not being used
		LOGD("android_handle_cmd() APP_CMD_LOST_FOCUS cmd begin");
		LOGD("android_handle_cmd() APP_CMD_LOST_FOCUS cmd end");
		}
		break;
	case APP_CMD_RESUME: {
		// 
		LOGD("android_handle_cmd() APP_CMD_RESUME cmd begin");
		// TODO, add pRenderer->Init() here
		LOGD("android_handle_cmd() APP_CMD_RESUME cmd end");
		}
		break;
	case APP_CMD_PAUSE: {
		//
		LOGD("android_handle_cmd() APP_CMD_PAUSE cmd begin");
		// TODO, add pRenderer->Destory() here
		LOGD("android_handle_cmd() APP_CMD_PAUSE cmd end");
		}
		break;
	}
}

AndroidTask::AndroidTask(android_app* pState, Renderer* pRenderer, unsigned int priority) :
	m_pState(pState),
	Task(priority)
{
    m_pEngine = new Engine();
    m_pEngine->setRenderer(pRenderer);

	m_pState->onAppCmd = ::android_handle_cmd;
	m_pState->onInputEvent = ::android_handle_input;
	m_pState->userData = (void*)m_pEngine;
}

AndroidTask::AndroidTask(AndroidPlatform* pPlatform, Renderer* pRenderer, unsigned int priority) :
	m_pState(pPlatform->getAppState()),
	Task(priority)
{
    m_pEngine = new Engine();
    m_pEngine->setRenderer(pRenderer);

    // todo, add some class for event handler, but not use static function
	m_pState->onAppCmd = ::android_handle_cmd;
	m_pState->onInputEvent = ::android_handle_input;
    m_pState->userData = (void*)m_pEngine;
}

AndroidTask::AndroidTask(const AndroidTask& _copy)
	:Task(_copy)
{
	m_pState = _copy.m_pState;
	m_pEngine = _copy.m_pEngine;
}

AndroidTask& AndroidTask::operator=(const AndroidTask& _assign)
{
	if (this == &_assign)
		return *this;
	m_pState = _assign.getAppState();
	m_pEngine = _assign.getEngine();
	m_priority = _assign.Priority();
	m_canKill = _assign.CanKill();
	return *this;
}

AndroidTask::~AndroidTask()
{
    // TODO, add code to check the pointer before delete
    if (m_pEngine)
    {
        delete m_pEngine;
        m_pEngine = NULL;
    }
}

bool AndroidTask::Start() {
	// Here is a good place to new the m_pScene of AndroidTask for current Scene, but not new Scene in AndroidApplication

    // TODO Here
    // SceneManager<GLScene>, scene tree, current scene, new scene, delete scene, load scene from outside files, delete them when goto other scene like menu, game scene, 
    // Scene->GLScene->Menu Scene -> Game Scene
    // GLBasicScene->Build(EGLRenderer)
    //                  -> new GLRenderable vector
    //                  -> assign to EGLRenderer

	// Start() of AndroidTask only need to feed or new the Renderer, but not Init() the Renderables in Scene

    // Scene only manage the memory of Renderables, but never call Renderer's Init()
    // Scene is a producer
    // Renderer is a consumer

    // Scene's Bind will assign it own Renderables Vector Pointer to Renderer's Renderables Vector Pointer
    // Scene should not have Init() or Destory() function, that belongs to Renderables
    // Scene should have Load() and UnLoad() for create new renderables and delete renderables

    // Rendererable's Init() will be called by Renderer's Init()
    // Renderer's Init() will be called when AndroidTask's android_handle_cmd()'s APP_CMD_INIT_WINDOW
    // Renderer's Destory() will be called when AndroidTask's android_handle_cmd()'s APP_CMD_TERM_WINDOW, etc

    // todo here, add some init job here for overall app, and only need to be run once
    // AndroidTask::Start() should be the very beginning of the native thread begin
    // Renderer's Init will be called when windows ready, hence we have the common idea: 
    //     Start()->Init()->Destroy()->Init()->Destory()->Stop()

    ANativeActivity* nativeActivity = m_pState->activity;
    AndroidAsset::setAssetManager(nativeActivity->assetManager);

    Scene* pScene = new GLBasicScene();
    m_pEngine->setScene(pScene);


	return true;
}

void AndroidTask::OnSuspend() {
}

void AndroidTask::Update() {
	int events;

	struct android_poll_source* pSource = NULL;
	int ident = ALooper_pollAll(0, 0, &events, (void**) &pSource);
	if (ident >= 0) {
		if (pSource) {
			pSource->process(this->m_pState, pSource);
		}
		if (this->m_pState->destroyRequested) {
			Kernel::getSingleton().setRunning(false);
		}
	}
}

void AndroidTask::OnResume() {
}

void AndroidTask::Stop() {
	// No need to call Renderable's Init() and Destory() herr, since Renderer task will call the same pointer of Scene's Renderables Vector Pointer's Renderable's Init()/Destory() in Renderer's own Init() and Destory()

    m_pEngine->getRenderer()->Bind(NULL);
    m_pEngine->getScene()->UnLoad();
    delete m_pEngine->getScene();
}

void AndroidTask::ClearClosing() {
	Kernel::getSingleton().setRunning(true);
}
