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

    GLuint getProgramHandle() const;

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

class GLMesh
    : public MeshRawData
{
public:
    GLMesh();
    virtual ~GLMesh();

protected:

private:

};

class GLInputVertexAttribute
    : public InputVertexAttribute
{
public:
    GLInputVertexAttribute(std::string name)
        : InputVertexAttribute(name),
          m_IAHandle(-1),
          m_IAElementNum(4),
          m_IAType(GL_FLOAT),
          m_IANormalized(GL_FALSE),
          m_IAStride(0),
          m_IAPointerOrOffset(0) 
    {};
    virtual ~GLInputVertexAttribute();

    bool Init(GLSLShader* shader);
    bool Setup(GLSLShader* shader);

    // getter
    GLuint getHandle() const { return m_IAHandle; }

    // setter
    void setElementNum(GLint val) { m_IAElementNum = val; };
    void setType(GLenum val) { m_IAType = val; }
    void setNormalized(GLboolean val) { m_IANormalized = val; };
    void setStride(GLsizei val) { m_IAStride = val; }
    void setPointerOrOffset(GLvoid* val) { m_IAPointerOrOffset = val; } 


private:
    GLuint m_IAHandle;

    GLint m_IAElementNum;
    GLenum m_IAType;
    GLboolean m_IANormalized;
    GLsizei m_IAStride;
    GLvoid* m_IAPointerOrOffset;
};

class GL_Renderable
{
public:
    explicit GL_Renderable(std::string name)
    {
        m_pGeometry = new Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>(name);
    }
    virtual ~GL_Renderable()
    {
        delete m_pGeometry;
    }

    // getter
    Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>* getGeometry() const { return m_pGeometry; }

    bool Init();
    bool Draw();
    bool Destroy();

private:
    Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>* m_pGeometry;

};

// TODO
// - Render Pass class
// - Scene class
// - 

#endif