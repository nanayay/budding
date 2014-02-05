#ifndef AY_ANDROID_TASK
#define AY_ANDROID_TASK

#include <android_native_app_glue.h>
#include "task.h"
#include "egl_renderer.h"

class AndroidTask: public Task {
private:
	android_app* m_pState;

public:
	explicit AndroidTask(android_app* pState, Renderer* pRenderer, unsigned int priority = Task::Priority::Normal);
	explicit AndroidTask(AndroidPlatform* pPlatform, Renderer* pRenderer, unsigned int priority = Task::Priority::Normal);
	AndroidTask(const AndroidTask& _copy);
	AndroidTask& operator=(const AndroidTask& _assign);
	virtual ~AndroidTask();

	android_app* GetAppState() const {
		return m_pState;
	}

	virtual bool Start();
	virtual void OnSuspend();
	virtual void Update();
	virtual void OnResume();
	virtual void Stop();

	static void ClearClosing();
};
#endif
