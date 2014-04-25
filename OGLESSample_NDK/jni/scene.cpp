#include "scene.h"

Scene::Scene()
{
    m_pRenderablesVector = new std::vector<Renderable*>();
}

Scene::~Scene()
{
    if (m_pRenderablesVector)
    {
        delete m_pRenderablesVector;
    }
    m_pRenderablesVector = NULL;
}
