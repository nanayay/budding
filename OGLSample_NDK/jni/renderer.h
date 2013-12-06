#ifndef AY_RENDERER
#define AY_RENDERER
#include "task.h"
#include "platform.h"

class Renderer
	: public Task
{
protected:
	bool m_bInitialized;
	bool m_bRendering;
	Platform* m_pPlatform;

public:
	virtual void RenderFrame() = 0;
	virtual void Init() = 0;
	virtual void Destroy() = 0;

public:
	explicit Renderer(Platform* pPlatform, unsigned int priority);
	virtual ~Renderer();

	virtual bool Start();
	virtual void OnSuspend();
	virtual void Update();
	virtual void OnResume();
	virtual void Stop();

	bool isInitialized() const { return this->m_bInitialized; }
	bool isRendering() const { return this->m_bRendering; }
	bool SetRendering(bool rendering) { this->m_bRendering = rendering; }
};
#endif
