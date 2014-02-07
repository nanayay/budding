#ifndef AY_RENDERABLE
#define AY_RENDERABLE

class Shader
{
public:
    explicit Shader(bool discard_shader_source = false);
    virtual ~Shader();

    bool Create();
    bool Enable();

public:
    bool isShaderSourceDiscardable() const { return m_isShaderSourceDiscardable; }

    bool isCreateOK() const { return m_isShaderCreateOK; }
    void setCreateOK(const bool isShaderCreateOK = false) { m_isShaderCreateOK = isShaderCreateOK; }
    bool isEnableOK() const { return m_isShaderEnableOK; }
    void setEnableOK(const bool isShaderEnableOK = false) { m_isShaderEnableOK = isShaderEnableOK; }

protected:
    virtual bool CompileAndLink() = 0;
    virtual void Setup() = 0;

protected:
    bool m_isShaderSourceDiscardable;
    bool m_isShaderCreateOK;
    bool m_isShaderEnableOK;

};

#endif