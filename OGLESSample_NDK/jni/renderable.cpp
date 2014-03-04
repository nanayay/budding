#include "renderable.h"

Shader::Shader(bool init_deep_copy_shader_source_str)
    :m_isInitDeepCopyShaderSourceStr(init_deep_copy_shader_source_str),
     m_isShaderCreateOK(false),
     m_isShaderEnableOK(false)
{
}

Shader::~Shader()
{
    // Notice, destructor can not call any kind of pure virtual function, and better not call virtual function
    /*
    if (isInitDeepCopyShaderSourceStr() == true)
    {
        DiscardShaderSource();
    }

    Remove();
    */
}

bool Shader::Create()
{
    if (isCreateOK() == false)
    {
        bool result = CompileAndLink();
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
        m_isShaderCreateOK = result;
    }
    return isCreateOK();
}

bool Shader::Enable()
{
    m_isShaderEnableOK = Setup();
    return isEnableOK();
}

bool Shader::Disable()
{
    bool result = UnSetup();
    m_isShaderEnableOK = !result;
    return result;
}
