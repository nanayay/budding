#include "gles_renderable.h"
#include <GLES/gl.h>

GLSLShader::GLSLShader(const std::string& vs_source, const std::string& fs_source, bool discard_shader_source = false)
    : Shader(discard_shader_source),
      m_vertexShaderId(0),
      m_fragmentShaderId(0),
      m_programId(0)
{
    m_pVertexShaderSource = new std::string(vs_source);
    m_pFragmentShaderSource = new std::string(fs_source);
}
GLSLShader::~GLSLShader()
{
    if (m_pVertexShaderSource)
    {
        delete m_pVertexShaderSource;
    }

    if (m_pFragmentShaderSource)
    {
        delete m_pFragmentShaderSource;
    }
}

bool GLSLShader::CompileAndLink()
{
    bool result = true;

    // todo: add handle for shader id, program id


    if (isShaderSourceDiscardable() == true)
    {
        delete m_pVertexShaderSource;
        m_pVertexShaderSource = NULL;
        delete m_pFragmentShaderSource;
        m_pFragmentShaderSource = NULL;
    }
    return result;
}

void GLSLShader::Setup()
{

}
