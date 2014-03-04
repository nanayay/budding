#ifndef AY_TYPE_DEFINES
#define AY_TYPE_DEFINES

// GL types and GL function headers are dependence on the different platform, but the name of them are all the same in all platform, so include different header in different platform, but keep the same GL code and logic

#if defined(__ANDROID__) || defined(ANDROID)
    #if 1
        #include <GLES2/gl2.h>
    #else
        #include <GLES/gl.h>
    #endif
    typedef long long TimeUnit;
    typedef unsigned char BYTE;

#elif defined(_WIN32) || defined(_WIN64)
    #error "NOT IMPLEMENT YET"

#elif defined(__APPLE__)
    #error "NOT IMPLEMENT YET"

#endif
    
#endif