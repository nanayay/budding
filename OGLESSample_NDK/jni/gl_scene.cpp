#include <algorithm>
#include "gl_scene.h"

bool GLBasicScene::Load()
{
    m_pGlClear = new GLClearRenderable();

    m_pGlClear->setClearColorRGB(0.0, 1.0, 1.0, 1.0);

    m_pRenderablesVector->push_back(m_pGlClear);

    return true;
}

bool GLBasicScene::UnLoad()
{
    if (m_pRenderablesVector)
    {
    for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
    {
        // TODO, check what will happen, when it's been Init() or Destroy() twice or more
        // TODO, check what will happen, when it's been delete twice or more
            if (*i)
            {
        (*i)->Destroy();
        delete (*i);
                (*i) = NULL;
            }
    }

    m_pRenderablesVector->erase(m_pRenderablesVector->begin(), m_pRenderablesVector->end());
    }
    return true;
}