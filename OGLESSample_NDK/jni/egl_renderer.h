#ifndef EGL_RENDERER
#define EGL_RENDERER

#include <EGL/egl.h>

#include "renderer.h"
#include "android_platform.h"
#include "type_defines.h"
#include "gl_renderable.h"

class EGLRenderer
	: public Renderer
{
private:
	EGLDisplay m_display;
	EGLSurface m_renderSurface;
	EGLContext m_context;

	EGLint m_width;
	EGLint m_height;

	android_app* m_pState;

public:
	explicit EGLRenderer(android_app* state, unsigned int priority = Task::Priority::Normal);
	explicit EGLRenderer(AndroidPlatform* pPlatform, unsigned int priority = Task::Priority::Normal);
	virtual ~EGLRenderer();

	virtual void RenderFrame();
	virtual void Init();
	virtual void Destroy();

};

#endif
