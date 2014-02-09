#include "gl_renderable.h"


GLSLShader::GLSLShader(const std::string& vs_source, const std::string& fs_source, bool discard_shader_source)
    : Shader(discard_shader_source),
      m_vertexShaderId(0),
      m_fragmentShaderId(0),
      m_programId(0),
      m_pVertexShaderSource(new std::string(vs_source)),
      m_pFragmentShaderSource(new std::string(fs_source))
{
}

GLSLShader::~GLSLShader()
{
    Remove();

    if (m_pVertexShaderSource)
    {
        delete m_pVertexShaderSource;
    }

    if (m_pFragmentShaderSource)
    {
        delete m_pFragmentShaderSource;
    }
}

void GLSLShader::LoadShader(GLuint shader_handle, std::string& shader_source)
{
   const unsigned int NUM_SHADERS = 1;
   const GLchar* pSourceCode = shader_source.c_str();
   GLint length = shader_source.length();

   glShaderSource(shader_handle, NUM_SHADERS, &pSourceCode, &length);
   glCompileShader(shader_handle);
}

bool GLSLShader::CompileAndLink()
{
    bool result = true;

    m_programId = glCreateProgram();

    m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    LoadShader(m_vertexShaderId, *m_pVertexShaderSource);
    glAttachShader(m_programId, m_vertexShaderId);

    m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    LoadShader(m_fragmentShaderId, *m_pFragmentShaderSource);
    glAttachShader(m_programId, m_fragmentShaderId);

    glLinkProgram(m_programId);

    //TODO, add glerror check for return value
    //maybe a class for glutil?

    return result;
}

bool GLSLShader::Setup()
{
    bool result = true;

    glUseProgram(m_programId);

    //TODO, add glerror check for return value
    //maybe a class for glutil?

    return result;
}



void GLSLShader::Remove()
{
    // TODO, remove the created shader handle, program id from system, for release system resource
}

void GLSLShader::DiscardShaderSource()
{
    if (isShaderSourceDiscardable() == true)
    {
        if (m_pVertexShaderSource)
        {
            delete m_pVertexShaderSource;
            m_pVertexShaderSource = NULL;
        }
        if (m_pFragmentShaderSource)
        {
            delete m_pFragmentShaderSource;
            m_pFragmentShaderSource = NULL;
        }
    }
}
