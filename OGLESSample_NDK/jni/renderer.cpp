#include "renderer.h"

Renderer::Renderer(unsigned int priority)
	: m_bInitialized(false),
	  m_bRendering(false),
	  m_pRenderablesVector(NULL),
	  Task(priority)
{
}

Renderer::~Renderer()
{
	if (m_bInitialized == true || m_bRendering == true)
	{
		this->Stop();
	}
}

bool Renderer::Start()
{
	return true;
}

void Renderer::OnSuspend()
{
	this->m_bRendering = false;
}

void Renderer::Update()
{
	if (this->m_bRendering == true && this->m_bInitialized == true)
	{
		this->RenderFrame();
	}
}

void Renderer::OnResume()
{
	this->m_bRendering = true;
}

void Renderer::Stop()
{
	this->m_bRendering = false;
	this->m_bInitialized = false;
	this->Destroy();
}

void Renderer::RenderFrame()
{
	if ( m_bInitialized == true && m_bRendering == true )
	{
		for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
		{
			(*i)->Draw();
		}
	}
}

void Renderer::Init()
{
	if (m_bInitialized == false)
	{
		for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
		{
			(*i)->Init();
		}

		m_bInitialized = true;
		m_bRendering = true;
	}
}

void Renderer::Destroy()
{
	for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
	{
		(*i)->Destroy();
	}
	m_bRendering = false;
	m_bInitialized = false;
}