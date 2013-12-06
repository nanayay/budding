#ifndef EGL_RENDERER
#define EGL_RENDERER

#include "renderer.h"
#include "android_platform.h"
#include <EGL/egl.h>
#include <GLES/gl.h>

class EGLRenderer
	: public Renderer
{
private:
	EGLDisplay m_display;
	EGLSurface m_renderSurface;
	EGLContext m_context;

	EGLint m_width;
	EGLint m_height;

public:
	explicit EGLRenderer(AndroidPlatform* pPlatform, unsigned int priority);
	virtual ~EGLRenderer();

	virtual void RenderFrame();
	virtual void Init();
	virtual void Destroy();

};

#endif
