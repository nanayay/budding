#include "scene.h"

Scene::Scene()
{
    m_pRenderablesVector = new std::vector<Renderable*>();
}

Scene::~Scene()
{
    delete m_pRenderablesVector;
}

bool Scene::Bind(Renderer* pRenderer)
{
    bool result = true;

    pRenderer->setRenderablesVectorPointer(m_pRenderablesVector);

    return result;
}

bool Scene::UnBind(Renderer* pRenderer)
{
    bool result = true;

    pRenderer->setRenderablesVectorPointer(NULL);

    return result;
}

bool Scene::Load()
{
    bool result = true;
    // Do what you have to do here
    return result;
}

bool Scene::UnLoad()
{
    bool result = true;
    // Do what you have to do here
    return result;
}
