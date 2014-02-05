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
		:Platform(copy_.GetPlatformName()),
		 m_pState(copy_.GetAppState())
	{
	}

	AndroidPlatform& operator=(const AndroidPlatform& assign_)
	{
		if (&assign_ != this)
		{
			SetPlatformName(assign_.GetPlatformName());
			m_pState = assign_.GetAppState();
		}

		return *this;
	}

	android_app* GetAppState() const { return m_pState; }
};

#endif
