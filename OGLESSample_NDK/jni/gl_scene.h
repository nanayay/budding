#ifndef AY_GL_SCENE
#define AY_GL_SCENE

#include "scene.h"
#include "gl_renderable.h"

class GLBasicScene
    : public Scene
{
public:
    GLBasicScene() {};
    virtual ~GLBasicScene() {};

protected:
    virtual bool Init();
    virtual bool Destroy();

    GLClearRenderable* m_pGlClear;

};

#endif