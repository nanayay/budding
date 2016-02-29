#ifndef AY_LOG
#define AY_LOG
#if defined(__ANDROID__) || defined(ANDROID)
    #include <android/log.h>

    #define TAG "OGLESSample"
    #define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
    #define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__))
    #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))
#elif defined(_WIN32) || defined(_WIN64)
    #pragma warning "NOT IMPLEMENT YET"
    #define LOGD
    #define LOGE
#elif defined(__APPLE__)
    #warning "NOT IMPLEMENT YET"
#endif

#endif
