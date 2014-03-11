#include "scene.h"

Scene::Scene()
{
    m_pRenderablesVector = new std::vector<Renderable*>();
}

Scene::~Scene()
{
    delete m_pRenderablesVector;
}

bool Scene::MakeCurrent(Renderer* pRenderer)
{
    bool result = true;

    pRenderer->setRenderablesVectorPointer(m_pRenderablesVector);

    return result;
}

bool Scene::Init()
{
    bool result = true;
    // Do what you have to do here
    return result;
}

bool Scene::Destroy()
{
    bool result = true;
    // Do what you have to do here
    return result;
}
