#include "kernel.h"
#include "android_task.h"
#include "log.h"

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
	case APP_CMD_INIT_WINDOW: {
		// get the window ready for showing
		Renderer* pRenderer = (Renderer*)app->userData;
		pRenderer->Init();
	}
		break;
	case APP_CMD_DESTROY: {
		//
		Renderer* pRenderer = (Renderer*)app->userData;
		pRenderer->Destroy();
	}
		break;
	case APP_CMD_TERM_WINDOW: {
		// clean up the window because it is being hidden/closed
        Renderer* pRenderer = (Renderer*)app->userData;
        pRenderer->Destroy();
	}
		break;
	case APP_CMD_RESUME: {
	}
		break;
	case APP_CMD_PAUSE: {
	}
		break;
	}
}

AndroidTask::AndroidTask(android_app* pState, Renderer* pRenderer, unsigned int priority) :
		m_pState(pState), Task(priority) {
	LOGD("Constructor android() begin");
	m_pState->onAppCmd = ::android_handle_cmd;
	m_pState->onInputEvent = ::android_handle_input;
	m_pState->userData = (void*)pRenderer;
	LOGD("Constructor android() end");
}

AndroidTask::AndroidTask(AndroidPlatform* pPlatform, Renderer* pRenderer, unsigned int priority) :
		m_pState(pPlatform->GetAppState()),
		Task(priority)
{
	LOGD("Constructor android() begin");
	m_pState->onAppCmd = ::android_handle_cmd;
	m_pState->onInputEvent = ::android_handle_input;
	m_pState->userData = (void*)pRenderer;
	LOGD("Constructor android() end");
}

AndroidTask::AndroidTask(const AndroidTask& _copy)
	:Task(_copy)
{
	this->m_pState = _copy.m_pState;
}

AndroidTask& AndroidTask::operator=(const AndroidTask& _assign)
{
	if (this == &_assign)
		return *this;
	m_pState = _assign.GetAppState();
	m_priority = _assign.Priority();
	m_canKill = _assign.CanKill();
	return *this;
}

AndroidTask::~AndroidTask() {
}

bool AndroidTask::Start() {
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
			Kernel::GetSingleton().SetRunning(false);
		}
	}
}

void AndroidTask::OnResume() {
}

void AndroidTask::Stop() {
}

void AndroidTask::ClearClosing() {
	Kernel::GetSingleton().SetRunning(true);
}
