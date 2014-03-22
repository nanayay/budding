#include "renderable.h"

bool ElementOfRenderable::Create()
{
    if (isCreateOK() == false)
    {
        bool result = false;
        // Do what you have to do, and,
        result = true;
        // Set result to true or false
        m_bIsCreateOK = result;
    }
    return isCreateOK();
}

bool ElementOfRenderable::Enable()
{
    if (isCreateOK() && isEnableOK() != true)
    {
        bool result = false;
        // Do what you have to do, and,
        result = true;
        // Set result to true or false
        m_bIsEnableOK = result;
    }
    return isEnableOK();
}

bool ElementOfRenderable::Disable()
{
    if (isCreateOK() && isEnableOK())
    {
        bool result = false;
        // Do what you have to do, and,
        result = true;
        // Set result to true or false
        m_bIsEnableOK = !result;
    }
    return !isEnableOK();
}

bool ElementOfRenderable::Dispose()
{
    if (isCreateOK())
    {
        bool result = false;
        // Do what you have to do, and,
        result = true;
        // Set result to true or false
        m_bIsCreateOK = !result;
    }
    return !isCreateOK();
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
    if (isCreateOK() && isEnableOK() != true)
    {
        bool result = false;
        result = Setup();
        m_bIsEnableOK = result;
    }
    return isEnableOK();
}

bool Shader::Disable()
{
    if (isCreateOK() && isEnableOK())
    {
        bool result = false;
        result = UnSetup();
        m_bIsEnableOK = !result;
    }
    return !isEnableOK();
}

bool Shader::Dispose()
{
    if (isCreateOK())
    {
        bool result = false;

        DiscardShaderSource();
        Remove();
        result = true;

        m_bIsCreateOK = !result;
    }
    return !isCreateOK();
}