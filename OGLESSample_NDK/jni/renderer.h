#ifndef AY_RENDERER
#define AY_RENDERER
#include <vector>
#include "task.h"
#include "renderable.h"
#include "scene.h"

class Scene;

class Renderer
	: public Task
{
protected:
	bool m_bInitialized;
	bool m_bRendering;

	// Find some way to manager the Renderable Vector in Renderer
	// - Renderer only use Renderable Vector, but not control it or own it
	// - Scene will manage the life cycle of Renderable Vector
	std::vector<Renderable*> * m_pRenderablesVector;

public:
	// setter and getter
	std::vector<Renderable*> * getRenderablsVectorPointer() const { return m_pRenderablesVector; }
	void setRenderablesVectorPointer(std::vector<Renderable*> * val) { m_pRenderablesVector = val; }

	// set to bind for Scene's Renderables Vector Pointer
	bool Bind(Scene* pScene);

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
	bool setRendering(bool rendering) { this->m_bRendering = rendering; }
};

#endif
