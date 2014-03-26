#ifndef AY_GL_RENDERABLE
#define AY_GL_RENDERABLE

#include <string>
#include "type_defines.h"
#include "renderable.h"
#include "log.h"

// glGetError
#ifdef GLDEBUG 
    #define CALL_GL(exp) {                                        \
    exp;                                                          \
    unsigned int err = GL_NO_ERROR;                               \
    do{                                                           \
          err = glGetError();                                     \
          if(err != GL_NO_ERROR){                                 \
               LOGE("glGetError() show error %d at %s at %d", err, __FILE__,__LINE__);\
          }                                                       \
     }while(err != GL_NO_ERROR);                                  \
    }

    #define FAILED_GL(res)  (res) != GL_NO_ERROR 

#else 
    #define CALL_GL(exp) exp 
#endif  

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
    virtual bool Dispose();

    // getter
    bool isUseCPUBuffer() const { return m_bIsUseCPUBuffer; }
    GLenum getPrimitiveTopologe(void) const { return m_PrimitiveTopologe; }
    GLenum getIndexDataType(void) const { return m_IndexType; }
    GLuint getVBOHandle(void) const { return m_vbo; }
    GLuint getIBOHandle(void) const { return m_ibo; }

    // setter
    void setPrimitiveTopologe(GLenum val) { m_PrimitiveTopologe = val; }
    void setIndexType(GLenum val) { m_IndexType = val; }

private:
    bool m_bIsUseCPUBuffer;

    GLenum m_PrimitiveTopologe;

    GLenum m_IndexType;

    GLuint m_vbo;
    GLuint m_ibo;

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
    GLuint LoadShader(GLenum shader_type, const std::string& shader_source);
    GLuint CreateProgram(const char* vertex_source, const char* fragment_source);

protected:
    const std::string* m_pVertexShaderSource;
    const std::string* m_pFragmentShaderSource;

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
    virtual bool Dispose();

    // getter
    GLuint getHandle() const { return m_IAHandle; }
    size_t getOffset() const { return (size_t)m_IAOffset; }

    // setter
    void setElementNum(GLint val) { m_IAElementNum = val; };
    void setType(GLenum val) { m_IAType = val; }
    void setNormalized(bool val) { m_IANormalized = val ? GL_TRUE : GL_FALSE; };
    void setStride(GLsizei val) { m_IAStride = val; }
    void setOffset(size_t val) { m_IAOffset = val; } 

private:
    GLuint m_IAHandle;

    GLint m_IAElementNum;
    GLenum m_IAType;
    GLboolean m_IANormalized;
    GLsizei m_IAStride;
    size_t m_IAOffset;

    // Note, IA only hold the offset of VertexAttribute in a vertex array buffer, but the mesh will hold the beginning of vertex array buffer
    // IA only hold the vertex attributes' offset, the real pointer will be get from Mesh
    // Mesh will hold the index's pointer or offset
    // Mesh will hold the vertex [all vertex attributes] array's beginning pointer, but the offset is in IA
    // IA's Enable() need both Mesh and Shader
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
    GLMesh* getMesh() const;
    GLSLShader* getShader() const;
    GLInputVertexAttribute* getInputVertexAttribute(std::string name) const;
    std::vector<GLInputVertexAttribute*> getAllInputVertexAttributes() const;

    // setter
    bool setMesh(GLMesh* val);
    bool setShader(GLSLShader* val);
    bool addInputVertexAttribute(GLInputVertexAttribute* val);

    virtual bool Init();
    virtual bool Draw();
    virtual bool Destroy();

protected:
    virtual bool BindElementOfRenderableToMe();

private:
    Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>* m_pGeometry;

};

class GLClearRenderable
    : public Renderable
{
public:
    GLClearRenderable();
    virtual ~GLClearRenderable() {};

    virtual bool Init();
    virtual bool Draw();
    virtual bool Destroy();

    // setter
    void setClearColorRGB(float r, float g, float b, float a)
    {
        m_fClearColorRed = r;
        m_fClearColorGreen = g;
        m_fClearColorBlue = b;
        m_fClearColorAlpha = a;
    }

    void setClearDepth(float d)
    {
        m_fClearDepth = d;
    }

    void setClearStencil(int s)
    {
        m_iClearStencil = s;
    }

    void setClearFlag(bool color, bool depth, bool stencil)
    {
        m_bIsClearColor = color;
        m_bIsClearDepth = depth;
        m_bIsClearStencil = stencil;
    }

protected:
    virtual bool BindElementOfRenderableToMe();

private:
    float m_fClearColorRed, m_fClearColorGreen, m_fClearColorBlue, m_fClearColorAlpha;
    float m_fClearDepth;
    int m_iClearStencil;

    bool m_bIsClearColor, m_bIsClearDepth, m_bIsClearStencil;

};

// TODO
// - Render Pass class
//     - 
// - Scene class
// - 
// - GLRenderable feed
//     - renderable->setMesh(val);
//     - renderable->getMesh()->setRenderable(renderable);

#endif