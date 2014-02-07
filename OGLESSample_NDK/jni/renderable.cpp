#include "renderable.h"

Shader::Shader(bool discard_shader_source)
    :m_isShaderSourceDiscardable(discard_shader_source),
     m_isShaderCreateOK(false),
     m_isShaderEnableOK(false),
{
}

Shader::~Shader() {}

bool Shader::Create()
{
    if (isCreateOK() == false)
    {
        bool result = CompileAndLink();
        setCreateOK(result);
    }
    return isCreateOK();
}

bool Shader::Enable()
{
    if (isEnableOK() == false)
    {
        bool result = Setup();
        setEnableOK(result);
    }
    return isEnableOK();
}
