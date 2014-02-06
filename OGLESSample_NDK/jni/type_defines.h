#ifndef AY_TYPE_DEFINES
#define AY_TYPE_DEFINES

#if defined(__ANDROID__) || defined(ANDROID)
    typedef long long TimeUnit;
#elif defined(_WIN32) || defined(_WIN64)

#elif defined(__APPLE__)

#endif
    
#endif