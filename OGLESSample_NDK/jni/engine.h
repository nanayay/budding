#ifndef AY_ENGINE
#define AY_ENGINE

#include "renderer.h"
#include "scene.h"

class Engine
{
public:
    explicit Engine() {};
    virtual ~Engine() {};

    Renderer* getRenderer() const { return m_pRenderer; }
    Scene* getScene() const { return m_pScene; }

    void setRenderer(Renderer* pRenderer) { m_pRenderer = pRenderer; }
    void setScene(Scene* pScene) { m_pScene = pScene; }

private:
    Scene* m_pScene;
    Renderer* m_pRenderer;

};

// todo notebook, adb logcat -c will clear all logs

#endif