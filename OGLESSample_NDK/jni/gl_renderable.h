#ifndef AY_GL_RENDERABLE
#define AY_GL_RENDERABLE

#include <string>
#include <utility>
#include <vector>

#include "type_defines.h"
#include "renderable.h"
#include "image.h"
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

class GLSampler
    : public TextureSampler
{
private:
    union _texParameterVal_
    {
        GLint m_texIntParameterVal;
        GLfloat m_texFloatParameterVal;
    };
    typedef std::pair<GLenum, _texParameterVal_> TexSamplerVal;
    typedef std::pair<bool, TexSamplerVal> SamplerVal;

public:
    explicit GLSampler(GLRenderable* renderable = NULL);
    virtual ~GLSampler();

    virtual bool Create();
    virtual bool Enable();
    virtual bool Disable();
    virtual bool Dispose();

    // setter
    void setSamplerTarget(GLenum val) { m_texTarget = val; }
    void addSamplerParameter(GLenum name, GLint val)
    {
        _texParameterVal_ t;
        t.m_texIntParameterVal = val;
        TexSamplerVal tval = TexSamplerVal(name, t);
        SamplerVal ttval = SamplerVal(true, tval);
        m_SamplerVal.push_back(ttval);
    }
    void addSamplerParameter(GLenum name, GLfloat val)
    {
        _texParameterVal_ t;
        t.m_texFloatParameterVal = val;
        TexSamplerVal tval = TexSamplerVal(name, t);
        SamplerVal ttval = SamplerVal(false, tval);
        m_SamplerVal.push_back(ttval);
    }

private:
    // for glTexParameter
    GLenum m_texTarget;
    std::vector<SamplerVal> m_SamplerVal;
    // todo, use tuple in c++11, but not pair with pair
};

// glBindTexture in OGLES 2.0 only support GL_TEXTURE_2D, or GL_TEXTURE_CUBE_MAP, hence we should have two class for texture, one is texture2d, another one is texturecubemap
class GLTexture
    : public Texture
{
public:
    // todo, add the texture_name_id be a int, that will gain a performance
    explicit GLTexture(const std::string& texture_name_id, const std::string& texture_uniform_name, const unsigned int texture_unit_id, GLSampler* texture_sampler = NULL);
    virtual ~GLTexture();

    // getter
    std::string getTextureUniformname() const { return m_uniformName; }
    GLuint  getTextureHandle() const { return m_texHandle; }
    // GLenum  getTextureTarget() const { return m_texTarget; }
    // GLint   getTextureInternalFormat() const { return m_internalFormat; }
    GLsizei getTextureWidth() const { return m_width; }
    GLsizei getTextureHeight() const { return m_height; }
    // GLint   getTextureBorder() const { return m_border; }
    // GLenum  getTextureFormat() const { return m_format; }
    // GLenum  getTextureType() const { return m_type; }

    // setter
    void setTextureUniformName(std::string uniform_name) { m_uniformName = uniform_name; }
    void setTextureUnitID(unsigned int val) { m_textureUnit = val; }
    // void setTextureTarget(GLenum val) { m_texTarget = val; }
    // void setTextureInternalFormat(GLint val) { m_internalFormat = val; }
    // void setTextureWidth(GLsizei val) { m_width = val; }
    // void setTextureHeight(GLsizei val) { m_height = val; }
    // void setTextureFormat(GLenum val) { m_format = val; }
    // void setTextureType(GLenum val) { m_type = val; }
    // void setTextureUnPackAlignmentNum(GLint val) { m_unPackAlignmentNum = val; }

    // Todo here, set all Element's class variable in constructor, make it be default somehow

protected:
    std::string m_uniformName;

    // for glActiveTexture
    unsigned int m_textureUnit;

    // for glGenTexture 
    GLuint m_texHandle;

    // for glTexImage2D
    GLenum m_texTarget;
    GLint m_internalFormat;
    GLsizei m_width;
    GLsizei m_height;
    GLenum m_format;
    GLenum m_type;

    // for glPixelStorei
    GLenum m_unPackAlignmentTarget; // UnPack Alignment Target is GL_UNPACK_ALIGNMENT as always
    GLint m_unPackAlignmentNum;

};

class GLTexture2D
    : public GLTexture
{
public:
    explicit GLTexture2D(const std::string& texture_name_id);
    explicit GLTexture2D(const std::string& texture_name_id, const std::string& texture_uniform_name, const unsigned int texture_unit_id, Image* pImage, GLSampler* texture_sampler = NULL, GLRenderable* renderable = NULL);
    virtual ~GLTexture2D();

    virtual bool Create();
    virtual bool Enable();
    virtual bool Disable();
    virtual bool Dispose();

private:
    bool LoadImage();
    bool UnloadImage();

    // todo here very, move m_pImage to super class
private:
    Image* m_pImage;

};

class GLRenderable
    : public Renderable
{
public:
    explicit GLRenderable(std::string name)
    {
        m_pGeometry = new Geometry<GLMesh, GLSLShader, GLSampler, GLTexture2D, GLInputVertexAttribute>(name);
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
    GLTexture2D* getTexture(std::string name) const;
    std::vector<GLTexture2D*> getAllTextures() const;

    // setter
    bool setMesh(GLMesh* val);
    bool setShader(GLSLShader* val);
    bool setSampler(GLSampler* val);
    bool addInputVertexAttribute(GLInputVertexAttribute* val);
    bool addTexture(GLTexture2D* val);

    virtual bool Init();
    virtual bool Draw();
    virtual bool Destroy();

protected:
    virtual bool BindElementOfRenderableToMe();

private:
    Geometry<GLMesh, GLSLShader, GLSampler, GLTexture2D, GLInputVertexAttribute>* m_pGeometry;

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

#endif
