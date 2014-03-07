#include "renderable.h"

bool ElementOfRenderable::Create()
{
    if (isCreateOK() == false)
    {
        bool result = false;
        // Do what you have to do, and,
        // Set result to true or false
        m_bIsCreateOK = result;
    }
    return isCreateOK();
}

bool ElementOfRenderable::Enable()
{
    bool result = false;
    // Do what you have to do, and,
    // Set result to true or false
    m_bIsEnableOK = result;
    return isEnableOK();
}

bool ElementOfRenderable::Disable()
{
    bool result = true;
    // Do what you have to do, and,
    // Set result to true or false
    m_bIsEnableOK = !result;
    return result;
}

Shader::Shader(bool init_deep_copy_shader_source_str)
    :m_isInitDeepCopyShaderSourceStr(init_deep_copy_shader_source_str)
{
}

Shader::~Shader()
{
    // Notice, destructor can not call any kind of pure virtual function, and better not call virtual function
}

bool Shader::Create()
{
    if (isCreateOK() == false)
    {
        bool result = false;
        result = CompileAndLink();
        if (isInitDeepCopyShaderSourceStr() == true)
        {
            if (result == true)
            {
                DiscardShaderSource();
            }
            else
            {
                // Do nothing when compiler error, but this class hold the deep copy for shader source
                // Here discard shader source is the de-constructor's job
            }
        }
        m_bIsCreateOK = result;
    }
    return isCreateOK();
}

bool Shader::Enable()
{
    m_bIsEnableOK = Setup();
    return isEnableOK();
}

bool Shader::Disable()
{
    bool result = UnSetup();
    m_bIsEnableOK = !result;
    return result;
}
