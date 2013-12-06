#ifndef AY_ANDROID_PLATFORM
#define AY_ANDROID_PLATFORM

#include "platform.h"
#include <android_native_app_glue.h>

class AndroidPlatform
	:public Platform
{
private:
	android_app* m_pState;

public:
	explicit AndroidPlatform(android_app* pState)
		:Platform("Android"),
		 m_pState(pState)
	{
	}

	virtual ~AndroidPlatform() {}

	void SetAppState(android_app* pState) { m_pState=pState; }
	android_app* GetAppState() const { return m_pState; }
};

#endif
