#include <algorithm>
#include "gl_scene.h"

bool GLBasicScene::Init()
{
    m_pGlClear = new GLClearRenderable();

    m_pGlClear->setClearColorRGB(1.0, 1.0, 0.0, 1.0);

    m_pRenderablesVector->push_back(m_pGlClear);

    return true;
}

bool GLBasicScene::Destroy()
{
    for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
    {
        (*i)->Destroy();
        delete (*i);
    }

    m_pRenderablesVector->erase(m_pRenderablesVector->begin(), m_pRenderablesVector->end());

    m_pGlClear = NULL;

    return true;
}