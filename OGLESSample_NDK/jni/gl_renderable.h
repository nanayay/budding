#ifndef AY_GL_RENDERABLE
#define AY_GL_RENDERABLE

#include <string>
#include "type_defines.h"
#include "renderable.h"

class GLSLShader
    : public Shader
{
public:
    explicit GLSLShader(const std::string* vs_source, const std::string* fs_source, bool init_deep_copy_shader_source_str = true);
    virtual ~GLSLShader();

protected:
    virtual bool CompileAndLink();
    virtual bool Setup();
    virtual bool UnSetup();
    virtual void Remove();
    virtual void DiscardShaderSource();

private:
    void LoadShader(GLuint shader_handle, const std::string& shader_source);

protected:
    const std::string* m_pVertexShaderSource;
    const std::string* m_pFragmentShaderSource;

    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;
    GLuint m_programId;
};

class GL_Renderable
{
public:
    GL_Renderable();
    virtual ~GL_Renderable();

private:
    GLSLShader* m_pShader;
    Geometry* m_pGeometry;

};

class GL_RenderPass
{
public:
    GL_RenderPass();
    virtual ~GL_RenderPass();

};

#endif