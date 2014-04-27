#include "type_defines.h"
#include "scene.h"

Scene::Scene()
{
    m_pRenderablesVector = new std::vector<Renderable*>();
}

Scene::~Scene()
{
    SAFE_DELETE(m_pRenderablesVector);
}
