#ifndef AY_RENDERER
#define AY_RENDERER
#include "task.h"

class Renderer
	: public Task
{
protected:
	bool m_bInitialized;
	bool m_bRendering;

public:
	virtual void RenderFrame() = 0;
	virtual void Init() = 0;
	virtual void Destroy() = 0;

public:
	explicit Renderer(unsigned int priority = Task::Priority::Normal);
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
