#include <android_native_app_glue.h>
#include "task.h"

class Android: public Task {
private:
	android_app* m_pState;

public:
	explicit Android(android_app* pState, unsigned int priority);
	virtual ~Android();

	android_app* GetAppState() {
		return m_pState;
	}

	virtual bool Start();
	virtual void OnSuspend();
	virtual void Update();
	virtual void OnResume();
	virtual void Stop();

	static void ClearClosing();
};
