#include "egl_renderer.h"
#include "log.h"
#include <assert.h>

EGLRenderer::EGLRenderer(android_app* state, unsigned int priority)
    :Renderer(priority),
     m_pState(state),
     m_display(EGL_NO_DISPLAY),
     m_renderSurface(EGL_NO_SURFACE),
     m_context(EGL_NO_CONTEXT),
     m_width(0),
     m_height(0)
{
}

EGLRenderer::EGLRenderer(AndroidPlatform* pPlatform, unsigned int priority)
	:Renderer(priority),
     m_pState(pPlatform->GetAppState()),
	 m_display(EGL_NO_DISPLAY),
	 m_renderSurface(EGL_NO_SURFACE),
	 m_context(EGL_NO_CONTEXT),
	 m_width(0),
	 m_height(0)
{
}

EGLRenderer::~EGLRenderer() {}

// TODO, try to have a class to feed the GLRenderable in EGLRenderer

void EGLRenderer::RenderFrame()
{
	if ( m_bInitialized == true && m_bRendering == true && m_width != 0 && m_height != 0)
	{
        //TODO, no need to add new Render_Pass class for glClear calls, just add a special empty GLRenderable to the first of m_pRenderables, which may called GL_ClearRenderable, which call glClear in its Draw() function
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Renderer::RenderFrame();

		eglSwapBuffers(m_display, m_renderSurface);
	}
}

void EGLRenderer::Init()
{
	if (m_pState->window == NULL)
	{
		LOGD("app_state->window are null, EGLRenderer can not Init() now");
		return;
	}

	LOGD("EGLRenderer Init() begin");

    LOGD("Init EGL begin");
	if (m_display == EGL_NO_DISPLAY)
	{
		m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		assert(m_display!= EGL_NO_DISPLAY);

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

		int windowFormat = ANativeWindow_getFormat(m_pState->window);
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

        EGLint max_swap_interval;
        res = eglGetConfigAttrib(m_display, config, EGL_MAX_SWAP_INTERVAL, &max_swap_interval);
        assert(res);

        EGLint min_swap_interval;
        res = eglGetConfigAttrib(m_display, config, EGL_MIN_SWAP_INTERVAL, &min_swap_interval);
        assert(res);

        // log the egl's max and min swap interval for eglSwapInterval
        LOGD("EGL Max Swap Interval is %d", max_swap_interval);
        LOGD("EGL Min Swap Interval is %d", min_swap_interval);

		int32_t setBufRes = ANativeWindow_setBuffersGeometry(m_pState->window, 0, 0, format);
		assert(setBufRes == 0);

		EGLNativeWindowType windowType;
		m_renderSurface = eglCreateWindowSurface(m_display, config, m_pState->window, NULL);
		assert(m_renderSurface != EGL_NO_SURFACE);
		LOGD("EGLRenderer eglCreateWindowSurface() success");

		EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
		assert(m_context != EGL_NO_CONTEXT);
		LOGD("EGLRenderer eglCreateContext() success");

		res = eglMakeCurrent(m_display, m_renderSurface, m_renderSurface, m_context);
		assert(res);
		LOGD("EGLRenderer eglMakeCurrent() success");

        res = eglQuerySurface(m_display, m_renderSurface, EGL_WIDTH, &m_width);
        assert(res);
        LOGD("EGLRenderer eglQuerySurface() success, m_width is %d", m_width);

        res = eglQuerySurface(m_display, m_renderSurface, EGL_HEIGHT, &m_height);
        assert(res);
        LOGD("EGLRenderer eglQuerySurface() success, m_height is %d", m_height);

        // try to disable vsync
        res = eglSwapInterval(m_display, 0);
        assert(res);
        LOGD("EGLRenderer eglSwapInterval to disable vsync success");
	}
    LOGD("Init EGL end");

    LOGD("super class Renderer::Init() begin");
    // TODO Here, try to feed renderer's renderable vector, other wise will show crash
    Renderer::Init();
    LOGD("super class Renderer::Init() end");

	LOGD("EGLRenderer Init() end");
}

void EGLRenderer::Destroy()
{
    LOGD("super class Renderer::Destroy() begin");
    Renderer::Destroy();
    LOGD("super class Renderer::Destroy() end");

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

}
