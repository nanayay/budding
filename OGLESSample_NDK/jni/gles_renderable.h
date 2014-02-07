#ifndef AY_GLES_RENDERABLE
#define AY_GLES_RENDERABLE

class GLSLShader
    : public Shader
{
public:
    explicit GLSLShader(const std::string& vs_source, const std::string& fs_source, bool discard_shader_source = false);
    virtual ~GLSLShader();

protected:
    virtual bool CompileAndLink();
    virtual void Setup();

protected:
    std::string* m_pVertexShaderSource;
    std::string* m_pFragmentShaderSource;

    GLuint m_vertexShaderId;
    GLuint m_fragmentShaderId;
    GLuint m_programId;

};

#endif