#ifndef AY_GL_RENDERABLE
#define AY_GL_RENDERABLE

#include <string>
#include "type_defines.h"
#include "renderable.h"

class GLRenderable;

class GLMesh
    : public MeshRawData
{
public:
    explicit GLMesh(bool use_cpu_buffer = false, GLRenderable* renderable = NULL);
    virtual ~GLMesh();

    virtual bool Create();
    virtual bool Enable();
    virtual bool Disable();

    // getter
    bool isUseCPUBuffer() const { return m_bIsUseCPUBuffer; }
    GLenum getPrimitiveTopologe(void) const { return m_PrimitiveTopologe; }
    GLenum getIndexType(void) const { return m_IndexType; }
    GLvoid* getIndexPointerOrOffset(void) const { return m_pIndexPointerOrOffset; }

    // setter
    void setPrimitiveTopologe(GLenum val) { m_PrimitiveTopologe = val; }
    void setIndexType(GLenum val) { m_IndexType = val; }

private:
    bool m_bIsUseCPUBuffer;

    GLenum m_PrimitiveTopologe;
    GLenum m_IndexType;
    GLvoid* m_pIndexPointerOrOffset;

};

class GLSLShader
    : public Shader
{
public:
    explicit GLSLShader(const std::string* vs_source, const std::string* fs_source, GLRenderable* renderable = NULL, bool init_deep_copy_shader_source_str = false);
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

class GLInputVertexAttribute
    : public InputVertexAttribute
{
public:
    GLInputVertexAttribute(std::string name, GLRenderable* renderable = NULL);
    virtual ~GLInputVertexAttribute();

    virtual bool Create();
    virtual bool Enable();
    virtual bool Disable();

    // getter
    GLuint getHandle() const { return m_IAHandle; }
    size_t getOffset() const { return (size_t)m_IAPointerOrOffset; }

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

class GLRenderable
    : public Renderable
{
public:
    explicit GLRenderable(std::string name)
    {
        m_pGeometry = new Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>(name);
    }
    virtual ~GLRenderable()
    {
        // TODO, check all the delete, make sure it check the pointer is not NULL before delete
        // TODO, check all the delete, make sure it assign it to NULL, after delete
        if (m_pGeometry != NULL)
        {
            delete m_pGeometry;
            m_pGeometry = NULL;
        }
    }

    // getter
    Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>* getGeometry() const { return m_pGeometry; }

    virtual bool Init();
    virtual bool Draw();
    virtual bool Destroy();

private:
    Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>* m_pGeometry;

};

// TODO
// - Render Pass class
//     - 
// - Scene class
// - 
// - GLRenderable feed
//     - renderable->getGeometry()->setMesh(val);
//     - renderable->getGeometry()->getMesh()->setRenderable(renderable);

#endif