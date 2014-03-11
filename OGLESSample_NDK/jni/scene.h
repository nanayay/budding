#ifndef AY_SCENE
#define AY_SCENE

#include "renderer.h"

class Scene
{
public:
    Scene();
    virtual ~Scene();

    virtual bool Init() = 0;
    virtual bool MakeCurrent(Renderer* pRenderer);
    virtual bool Destroy() = 0;

    // Scene own the life cycle of Renderable Vector
    // One Scene match to one Renderable Vector, in which contains what need to be draw

    // getter
    std::vector<Renderable*> * getRenderablesVectorPointer() const { return m_pRenderablesVector; }

protected:
    std::vector<Renderable*> * m_pRenderablesVector;
};

#endif