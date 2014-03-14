#ifndef AY_GL_SCENE
#define AY_GL_SCENE

#include "scene.h"
#include "gl_renderable.h"

class GLBasicScene
    : public Scene
{
public:
    GLBasicScene() {};
    virtual ~GLBasicScene() { UnLoad(); };

protected:
    virtual bool Load();
    virtual bool UnLoad();

};

#endif