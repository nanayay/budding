#include <assert.h>
#include "renderer.h"
#include "log.h"

Renderer::Renderer(unsigned int priority)
	: m_bInitialized(false),
	  m_bRendering(false),
	  m_pRenderablesVector(NULL),
	  Task(priority)
{
}

Renderer::~Renderer()
{
	m_bInitialized = false;
	m_bRendering = false;
	m_pRenderablesVector = NULL;
}

bool Renderer::Start()
{
	// Start() will be called before Renderer been added to Kernel's task list, if return true, it will be added to the list
	// Start() means whether this task can be started
	// Renderer::Start() give you some chance to prepare something before it really runes, for example, AndroidTask's malloc/new m_pScene, and feed it
	// Renderer::Start() will be called when it begin to add to Kernel's task list
	// Renderer::Init() will be called several times, for example, you paused or resumed from other activity [phone call, maybe], you need to recall Init(), but not Start()
	// Renderer::Init() will be called in AndroidTask's cmd_handle's message handle when receive the message to APP_CMD_INIT_WINDOW
	// If it's not a Task sub-class, it is easy to call Init(), since it did not have Start() function
	return true;
}

void Renderer::OnSuspend()
{
	this->m_bRendering = false;
	// m_pRenderablesVector depends outside Scene, hence Renderer's OnSuspend() has no responsibility to assign it to NULL
}

void Renderer::Update()
{
	if (this->m_bRendering == true && this->m_bInitialized == true && this->m_pRenderablesVector != NULL)
	{
		RenderFrame();
	}
	else
	{
		LOGD("Find Renderer's m_bRendering or m_bInitialized or m_pRenderablesVector should be false or NULL in Renderer::Update");
		LOGD("That will make no RenderFrame() be called");
	}
}

void Renderer::OnResume()
{
	this->m_bRendering = true;
	// m_pRenderablesVector depends outside Scene, hence Renderer's OnResume() has no responsibility to assign it to some value
}

void Renderer::Stop()
{
	this->m_bRendering = false;
	this->m_bInitialized = false;
	this->Destroy();
}

void Renderer::RenderFrame()
{
	if ( m_bInitialized == true && m_bRendering == true && m_pRenderablesVector != NULL)
	{
		for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
		{
			(*i)->Draw();
		}
	}
}

void Renderer::Init()
{
	LOGD("Renderer::Init begin");

	if (m_bInitialized == false)
	{
		if (m_pRenderablesVector != NULL)
		{
			for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
			{
				if ((*i) != NULL)
				{
					(*i)->Init();
				}
				else
				{
					LOGE("Renderer::Init() is calling its Renderables Pointer Vector, but it contain some NULL pointer, which should be a error");
					assert(0);
				}
			}
		}
		else
		{
			LOGD("Find m_pRenderablesVector is NULL, hence Renderer::Init() will not call its Init()");
		}

		m_bInitialized = true;
		m_bRendering = true;
	}

	LOGD("Renderer::Init end");
}

void Renderer::Destroy()
{
	if (m_pRenderablesVector != NULL)
	{
		for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
		{
			if ((*i) != NULL)
			{
				(*i)->Destroy();
			}
		}
	}
	m_bRendering = false;
	m_bInitialized = false;
}

bool Renderer::Bind(Scene* pScene)
{
	bool result = true;

	setRenderablesVectorPointer(pScene->getRenderablesVectorPointer());

	return result;
}