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
	}
		break;
	case APP_CMD_DESTROY: {
		//
	}
		break;
	case APP_CMD_TERM_WINDOW: {
		// clean up the window because it is being hidden/closed
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

Android::Android(android_app* pState, unsigned int priority) :
		m_pState(pState), Task(priority) {
	LOGD("Constructor android() begin");
	m_pState->onAppCmd = ::android_handle_cmd;
	m_pState->onInputEvent = ::android_handle_input;
	LOGD("Constructor android() end");
}

Android::~Android() {
}

bool Android::Start() {
	return true;
}

void Android::OnSuspend() {

}

void Android::Update() {
	int events;

	struct android_poll_source* pSource = NULL;
	int ident = ALooper_pollAll(0, 0, &events, (void**) &pSource);
	if (ident >= 0) {
		if (pSource) {
			pSource->process(this->m_pState, pSource);
		}
		if (this->m_pState->destroyRequested) {
			Kernel::GetSingletonKernel().SetRunning(false);
		}
	}
}

void Android::OnResume() {
}

void Android::Stop() {
}

void Android::ClearClosing() {
	Kernel::GetSingletonKernel().SetRunning(true);
}
