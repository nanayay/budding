#include "renderer.h"

Renderer::Renderer(Platform* pPlatform, unsigned int priority)
	: m_bInitialized(false),
	  m_bRendering(false),
	  m_pPlatform(pPlatform),
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
	if (this->m_bInitialized == false)
	{
		this->Init();
	}

	if (this->m_bRendering == true)
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
