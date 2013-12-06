#include <jni.h>
#include <android_native_app_glue.h>
#include "log.h"
#include "kernel.h"
#include "android_task.h"
#include "android_platform.h"
#include "egl_renderer.h"

void android_main(struct android_app* state) {
	LOGD("app_dummy");
	app_dummy();

	LOGD("EGLRenderer create and add to kernel begin");
	AndroidPlatform* pAndroidPlatform = new AndroidPlatform(state);
	Renderer* pEGLRenderer = new EGLRenderer(pAndroidPlatform, 5);
	Kernel::GetSingleton().AddTask(pEGLRenderer);
	LOGD("EGLRenderer create and add to kernel end");

	LOGD("android task create and add to kernel begin");
	AndroidTask* pAndroidTask = new AndroidTask(pAndroidPlatform, pEGLRenderer, 5);
	Kernel::GetSingleton().AddTask(pAndroidTask);
	LOGD("android task create and add to kernel end");

	LOGD("kernel execute begin");
	Kernel::GetSingleton().Execute();
	LOGD("kernel execute end");

	LOGD("android clear closing begin");
	delete pAndroidPlatform;
	AndroidTask::ClearClosing();
	LOGD("android clear closing end");

}

