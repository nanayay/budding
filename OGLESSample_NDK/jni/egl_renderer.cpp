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
     m_pState(pPlatform->getAppState()),
	 m_display(EGL_NO_DISPLAY),
	 m_renderSurface(EGL_NO_SURFACE),
	 m_context(EGL_NO_CONTEXT),
	 m_width(0),
	 m_height(0)
{
}

EGLRenderer::~EGLRenderer()
{
   if (m_bInitialized || m_bRendering)
    {
        Destroy();
    } 
}

void EGLRenderer::RenderFrame()
{
    LOGD("EglRenderer::RenderFrame() begin");
	if ( m_bInitialized == true && m_bRendering == true && m_width != 0 && m_height != 0)
	{
        // No need to add Render Pass for glClear calls, just add a special Renderable class, aka, GLClearRenderable to the first of vector in Renderer class, which named m_pRenderablesVector
        // In GLClearRenderable's Draw(), it will call glClear

        LOGD("EglRenderer::RenderFrame() is calling Renderer::RenderFrame()");
        Renderer::RenderFrame();

		eglSwapBuffers(m_display, m_renderSurface);

	}
    LOGD("EglRenderer::RenderFrame() end");
}

void EGLRenderer::Init()
{
    LOGD("EGLRenderer Init() begin");

	if (m_pState->window == NULL)
	{
		LOGD("app_state->window are null, EGLRenderer can not Init() now");
		return;
	}

    LOGD("Init EGL begin");
	if (m_display == EGL_NO_DISPLAY)
	{
		m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		assert(m_display!= EGL_NO_DISPLAY);

        EGLint majorEGLVersion, minorEGLVersion;
		EGLBoolean res = eglInitialize(m_display, &majorEGLVersion, &minorEGLVersion);
		assert(res);
        LOGD("EGL Major Version is %d", majorEGLVersion);
        LOGD("EGL Minor Version is %d", minorEGLVersion);

        EGLint OGLESVersionToken;

        #if defined(USE_GLES3)
            // OGLESVersionToken = EGL_OPENGL_ES3_BIT_KHR; // Don't use EGL_OPENGL_ES3_BIT_KHR, since it only support in EGL 1.4 and we don't have it right now
            OGLESVersionToken = EGL_OPENGL_ES2_BIT;
        #elif defined(USE_GLES2)
            OGLESVersionToken = EGL_OPENGL_ES2_BIT;
        #elif defined(USE_GLES1)
            OGLESVersionToken = EGL_OPENGL_ES_API;
        #else
            OGLESVersionToken = EGL_OPENGL_ES2_BIT;
        #endif

		const EGLint RGBX_8888_ATTRIBS[]=
		{
				EGL_RENDERABLE_TYPE, OGLESVersionToken,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_RED_SIZE,	8,
				EGL_GREEN_SIZE, 8,
				EGL_BLUE_SIZE,  8,
				EGL_DEPTH_SIZE,	8,
				EGL_NONE
		};
		const EGLint RGB_565_ATTRIBS[]=
		{
				EGL_RENDERABLE_TYPE, OGLESVersionToken,
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

        LOGD("EGL Max Swap Interval is %d", max_swap_interval);
        LOGD("EGL Min Swap Interval is %d", min_swap_interval);

		int32_t setBufRes = ANativeWindow_setBuffersGeometry(m_pState->window, 0, 0, format);
		assert(setBufRes == 0);

		EGLNativeWindowType windowType;
		m_renderSurface = eglCreateWindowSurface(m_display, config, m_pState->window, NULL);
		assert(m_renderSurface != EGL_NO_SURFACE);
		LOGD("EGLRenderer eglCreateWindowSurface() success");

        EGLint OGLESVersion;

        #if defined(USE_GLES3)
            OGLESVersion = 3;
        #elif defined(USE_GLES2)
            OGLESVersion = 2;
        #elif defined(USE_GLES1)
            OGLESVersion = 1;
        #else
            OGLESVersion = 2;
        #endif

		EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, OGLESVersion, EGL_NONE };
		m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttribs);
		assert(m_context != EGL_NO_CONTEXT);
		LOGD("EGLRenderer eglCreateContext() success");

		res = eglMakeCurrent(m_display, m_renderSurface, m_renderSurface, m_context);
		assert(res);
		LOGD("EGLRenderer eglMakeCurrent() success");

        const GLubyte* glVendor;
        const GLubyte* glRenderer;
        const GLubyte* glVersion;

        glVendor = glGetString(GL_VENDOR);
        glRenderer = glGetString(GL_RENDERER);
        glVersion = glGetString(GL_VERSION);

        LOGD("The current GL_VENDOR: %s", glVendor);
        LOGD("The current GL_RENDERER: %s", glRenderer);
        LOGD("The current GL_VERSION: %s", glVersion);

        #if defined(USE_GLES3)
            GLint glMajorVersion, glMinorVersion;
            glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
            glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);
            LOGD("These query methods only available in OGL ES 3:");
            LOGD("    - The current OGL Context support OGL Major_Version: %d", glMajorVersion);
            LOGD("    - The current OGL Context support OGL Minor_Version: %d", glMinorVersion);
        #endif

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
    Renderer::Init();
    LOGD("super class Renderer::Init() end");

	LOGD("EGLRenderer Init() end");
}

void EGLRenderer::Destroy()
{
    LOGD("Renderer::Destroy() begin");
    Renderer::Destroy();
    LOGD("Renderer::Destroy() end");

    LOGD("EGLRenderer::Destroy() end");
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
    LOGD("EGLRenderer::Destroy() end");

}
