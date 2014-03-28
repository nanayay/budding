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

    bool Create();
    bool Enable();
    bool Disable();
    bool Dispose();

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

// TODO here, glBindTexture in OGLES 2.0 only support GL_TEXTURE_2D, or GL_TEXTURE_CUBE_MAP, hence we should have two class for texture, one is texture2d, another one is texturecubemap
class GLTexture
    : public Texture
{
public:
    explicit GLTexture(std::string uniform_name, GLRenderable* renderable = NULL);
    virtual ~GLTexture();

    virtual bool Create();
    virtual bool Enable();
    virtual bool Disable();
    virtual bool Dispose();

    // getter
    GLuint  getTextureHandle() const { return m_textureHandle; }
    GLenum  getTextureTarget() const { return m_texTarget; }
    // GLint   getTextureMipLevelsNum() const { return m_mipLevels; }
    // GLint   getTextureInternalFormat() const { return m_internalFormat; }
    GLsizei getTextureWidth() const { return m_width; }
    GLsizei getTextureHeight() const { return m_height; }
    // GLint   getTextureBorder() const { return m_border; }
    // GLenum  getTextureFormat() const { return m_format; }
    // GLenum  getTextureType() const { return m_type; }
    // void*   getTextureData() const { return m_pImageData; }

    // setter
    void setTextureTarget(GLenum val) { m_texTarget = val; }
    void setTextureMipLevelsNum(GLint val) { m_mipLevels = val; }
    void setTextureInternalFormat(GLint val) { m_internalFormat = val; }
    void setTextureWidth(GLsizei val) { m_width = val; }
    void setTextureHeight(GLsizei val) { m_height = val; }
    void setTextureFormat(GLenum val) { m_format = val; }
    void setTextureType(GLenum val) { m_type = val; }
    void setTextureData(void* val) { m_pImageData = val; }
    void setTextureUnPackAlignmentNum(GLint val) { m_unPackAlignmentNum = val; }
    void setTextureSampler(GLSampler* val) { m_pSampler = val; }

private:
    // for glTexImage2D
    GLuint m_textureHandle;
    GLenum m_texTarget; // todo here, set it as default = GL_TEXTURE_2D;
    GLint m_mipLevels;
    GLint m_internalFormat;
    GLsizei m_width;
    GLsizei m_height;
    const GLint m_border = 0;
    GLenum m_format;
    GLenum m_type;
    void* m_pImageData;

    // for glPixelStorei
    const GLenum m_unPackAlignmentTarget = GL_UNPACK_ALIGNMENT;
    GLint m_unPackAlignmentNum;

    // for texture's sampler
    GLSampler* m_pSampler;

    // Todo here
    // Add Sampler object

    // Todo here
    // only make sure texture object is not null, then set it

    // todo here
    // if sampler is null, that means no need to set, just use default value
    // if renderable has sampler also not null, set renderable's sampler first, then when goto texture's sampler is also not null, set texture's sample also

};

class GLSampler
    : public TextureSampler
{
public:
    explicit GLSampler(GLRenderable* renderable = NULL);
    virtual ~GLSampler();

    virtual bool Create();
    virtual bool Enable();
    virtual bool Disable();
    virtual bool Dispose();

    // setter
    void setSamplerTarget(GLenum val) { m_texTarget = val; }
    void setSamplerParameter(GLenum name, GLint val) { m_texParameterName = name; m_texParameterVal.m_texIntParameterVal = val; }
    void setSamplerParameter(GLenum name, GLfloat val) { m_texParameterName = name; m_texParameterVal.m_texFloatParameterVal = val; }

private:
    // for glTexParameter
    GLenum m_texTarget;
    GLenum m_texParameterName;
    union
    {
        GLint m_texIntParameterVal;
        GLfloat m_texFloatParameterVal;
    } m_texParameterVal;
};


class GLRenderable
    : public Renderable
{
public:
    explicit GLRenderable(std::string name)
    {
        m_pGeometry = new Geometry<GLMesh, GLSLShader, GLSampler, GLTexture, GLInputVertexAttribute>(name);
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
    GLSampler* getSampler() const;
    GLInputVertexAttribute* getInputVertexAttribute(std::string name) const;
    std::vector<GLInputVertexAttribute*> getAllInputVertexAttributes() const;
    GLTexture* getTexture(std::string name) const;
    std::vector<GLTexture*> getAllTextures() const;

    // setter
    bool setMesh(GLMesh* val);
    bool setShader(GLSLShader* val);
    bool setSampler(GLSampler* val);
    bool addInputVertexAttribute(GLInputVertexAttribute* val);
    bool addTexture(GLTexture* val);

    virtual bool Init();
    virtual bool Draw();
    virtual bool Destroy();

    // todo here
    // add check to render, in which texture's array should be no zero size, then apply the texture, sampler is not null, then apply the sampler


private:
    Geometry<GLMesh, GLSLShader, GLSampler, GLTexture, GLInputVertexAttribute>* m_pGeometry;

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

private:
    float m_fClearColorRed, m_fClearColorGreen, m_fClearColorBlue, m_fClearColorAlpha;
    float m_fClearDepth;
    int m_iClearStencil;

    bool m_bIsClearColor, m_bIsClearDepth, m_bIsClearStencil;

};

#endif