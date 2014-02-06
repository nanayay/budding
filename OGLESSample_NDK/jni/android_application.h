#ifndef AY_ANDROID_APPLICATION
#define AY_ANDROID_APPLICATION
#include <android_native_app_glue.h>
#include "application.h"

class AndroidApplication
    : public Application
{
public:
    AndroidApplication(android_app* state);
    virtual ~AndroidApplication();

    virtual bool Initialize();
    virtual void Run();

private:
    android_app* m_pState;

    bool AddTasks();
};

#endif