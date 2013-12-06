#include <jni.h>
#include <android_native_app_glue.h>
#include "log.h"
#include "kernel.h"
#include "android_task.h"

void android_main(struct android_app* state) {
	LOGD("app_dummy");
	app_dummy();

	LOGD("kernel begin");
	Kernel kernel;
	LOGD("kernel end");

	LOGD("android task begin");
	Android* p_android_task = new Android(state, 10);
	LOGD("android task end");

	LOGD("kernel add task begin");
	kernel.AddTask(p_android_task);
	LOGD("kernel add task end");

	LOGD("kernel execute begin");
	kernel.Execute();
	LOGD("kernel execute end");

	LOGD("android clear closing begin");
	Android::ClearClosing();
	LOGD("android clear closing end");

}

