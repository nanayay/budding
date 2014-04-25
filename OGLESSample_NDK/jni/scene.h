#ifndef AY_SCENE
#define AY_SCENE

#include "renderer.h"

class Renderer;

class Scene
{
public:
    explicit Scene();
    virtual ~Scene();

    virtual bool Load() = 0;
    virtual bool UnLoad() = 0;

    // Scene own the life cycle of Renderable Vector
    // But Scene is only a producer of Renderables
    // And Renderer is a consumer of Renderables

    // One Scene match to one Renderable Vector, in which contains what need to be draw

    // getter
    std::vector<Renderable*> * getRenderablesVectorPointer() const { return m_pRenderablesVector; }

protected:
    std::vector<Renderable*> * m_pRenderablesVector;
};

#endif