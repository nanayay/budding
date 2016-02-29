#ifndef AY_ANDROID_PLATFORM
#define AY_ANDROID_PLATFORM

#include "platform.h"
#include <jni.h>
#include <android_native_app_glue.h>

class AndroidPlatform
	:public Platform
{
private:
	android_app* m_pState;

public:
	explicit AndroidPlatform(android_app* const pState)
		:Platform("Android"),
		 m_pState(pState)
	{
	}

	virtual ~AndroidPlatform() {}

	AndroidPlatform(const AndroidPlatform& copy_)
		:Platform(copy_.getPlatformName()),
		 m_pState(copy_.getAppState())
	{
	}

	AndroidPlatform& operator=(const AndroidPlatform& assign_)
	{
		if (&assign_ != this)
		{
			setPlatformName(assign_.getPlatformName());
			m_pState = assign_.getAppState();
		}

		return *this;
	}

	android_app* getAppState() const { return m_pState; }
};

#endif
