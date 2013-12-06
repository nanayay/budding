#include "egl_renderer.h"
#include <assert.h>

EGLRenderer::EGLRenderer(AndroidPlatform* pPlatform, unsigned int priority)
	:Renderer(pPlatform, priority),
	 m_display(EGL_NO_DISPLAY),
	 m_renderSurface(EGL_NO_SURFACE),
	 m_context(EGL_NO_CONTEXT),
	 m_width(0),
	 m_height(0)
{
	LOGD("EGLRenderer constructor begin");
	LOGD("EGLRenderer constructor end");
}

EGLRenderer::~EGLRenderer()
{
}

void EGLRenderer::RenderFrame()
{
	LOGD("EGLRenderer RenderFrame begin");
	if (m_bRendering == true && m_width != 0 && m_height != 0)
	{
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		eglSwapBuffers(m_display, m_renderSurface);
	}
	LOGD("EGLRenderer RenderFrame end");
}

void EGLRenderer::Init()
{
	LOGD("EGLRenderer Init begin");
	AndroidPlatform* pPlatform = (AndroidPlatform*)m_pPlatform;
	/*
	if (m_display == EGL_NO_DISPLAY)
	{
		m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		assert(m_display);
		EGLBoolean res = eglInitialize(m_display, NULL, NULL);
		assert(res);

		const EGLint RGBX_8888_ATTRIBS[]=
		{
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_RED_SIZE,	8,
				EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE,  8,
				EGL_DEPTH_SIZE,	8,
				EGL_NONE
		};
		const EGLint RGB_565_ATTRIBS[]=
		{
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_RED_SIZE, 5,
				EGL_GREEN_SIZE, 6,
				EGL_BLUE_SIZE, 5,
				EGL_DEPTH_SIZE, 8,
				EGL_NONE
		};

		const EGLint* attribList;
		android_app* p_android_app_state = pPlatform->GetAppState();

		int windowFormat = ANativeWindow_getFormat(p_android_app_state->window);
		if ((windowFormat == WINDOW_FORMAT_RGBA_8888) || (windowFormat == WINDOW_FORMAT_RGBX_8888))
		{
			attribList = RGBX_8888_ATTRIBS;
		}
		else
		{
			attribList = RGB_565_ATTRIBS;
		}
		EGLConfig config;
		EGLint numConfigs;
		res = eglChooseConfig(m_display, attribList, &config, 1, &numConfigs);
		assert(res);

		EGLint format;
		res = eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &format);
		assert(res);

		int32_t setBufRes = ANativeWindow_setBuffersGeometry(p_android_app_state->window, 0, 0, format);
		assert(setBufRes == 0);

		EGLNativeWindowType windowType;
		m_renderSurface = eglCreateWindowSurface(m_display, config, p_android_app_state->window, NULL);

		EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
		assert(m_context != EGL_NO_CONTEXT);

		res = eglMakeCurrent(m_display, m_renderSurface, m_renderSurface, m_context);
		assert(res);

        res = eglQuerySurface(m_display, m_renderSurface, EGL_WIDTH, &m_width);
        assert(res);

        res = eglQuerySurface(m_display, m_renderSurface, EGL_HEIGHT, &m_height);
        assert(res);

        m_bInitialized = true;
	}
	*/
	LOGD("EGLRenderer Init end");
}

void EGLRenderer::Destroy()
{
    if (m_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(m_display, m_context);
        }
        if (m_renderSurface != EGL_NO_SURFACE)
        {
            eglDestroySurface(m_display, m_renderSurface);
        }
        eglTerminate(m_display);
    }
    m_display = EGL_NO_DISPLAY;
    m_context = EGL_NO_CONTEXT;
    m_renderSurface = EGL_NO_SURFACE;
    m_width = 0;
    m_height = 0;
    m_bRendering = false;
    m_bInitialized = false;

}
