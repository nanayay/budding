#include <android/log.h>

#ifndef AY_LOG
#define AY_LOG

#define TAG "OGLSample"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
#define LGOE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

#endif
