#ifndef AY_TYPE_DEFINES
#define AY_TYPE_DEFINES

// GL types and GL function headers are dependence on the different platform, but the name of them are all the same in all platform, so include different header in different platform, but keep the same GL code and logic

#if defined(__ANDROID__) || defined(ANDROID)
    #if defined(USE_GLES3) 
        #include <GLES3/gl3.h>
        #include <GLES3/gl3ext.h>
    #elif defined(USE_GLES2)
        #include <GLES2/gl2.h>
        #include <GLES2/gl2ext.h>
    #elif defined(UES_GLES1)
        #include <GLES/gl.h>
        #include <GLES/glext.h>
    #else
        #include <GLES2/gl3.h>
        #include <GLES2/gl3ext.h>
    #endif

    typedef long long TimeUnit;
    typedef unsigned char BYTE;

#elif defined(_WIN32) || defined(_WIN64)
    #error "NOT IMPLEMENT YET"

#elif defined(__APPLE__)
    #error "NOT IMPLEMENT YET"

#endif

#ifndef SAFE_DELETE
    #define SAFE_DELETE(p)  { if (p) { delete (p); (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
    #define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p)=NULL; } }
#endif
    
#endif