#ifndef AY_GL_SCENE
#define AY_GL_SCENE

#include "scene.h"
#include "gl_renderable.h"

class GLBasicScene
    : public Scene
{
public:
    explicit GLBasicScene();
    virtual ~GLBasicScene();

    virtual bool Load();
    virtual bool UnLoad();

};

#endif