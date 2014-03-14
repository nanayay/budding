#include "gl_renderable.h"
#include "log.h"

GLMesh::GLMesh(bool use_cpu_buffer, GLRenderable* renderable)
    : MeshRawData(),
      m_bIsUseCPUBuffer(use_cpu_buffer)
{
    m_pRenderable= renderable;
}

GLMesh::~GLMesh()
{
}

bool GLMesh::Create()
{
    if (isCreateOK() == false)
    {
        bool result = false;

        if (m_bIsUseCPUBuffer == true)
        {
            // use vertex and index in CPU memory
            // vertex array buffer's cpu memory pointer will be set in IA's Enable()

            // index
            this->m_pIndexPointerOrOffset = this->getIndexDataPointer();

        }
        else
        {
            // use vertex and index buffer in GPU memory
            // TODO Here
            // create buffer of GPU for this

            // TODO Here, for test
            // write some class to feed shader, mesh, IA, and combine a renderable, assign to renderer's list
        }

        m_bIsCreateOK = result;
    }
    return isCreateOK();
}

bool GLMesh::Enable()
{
    bool result = false;
    // Do what you have to do, and,
    // Set result to true or false
    m_bIsEnableOK = result;
    return isEnableOK();
}

bool GLMesh::Disable()
{
    bool result = true;
    // Do what you have to do, and,
    // Set result to true or false
    m_bIsEnableOK = !result;
    return result;    
}


GLSLShader::GLSLShader(const std::string* vs_source, const std::string* fs_source, GLRenderable* renderable, bool init_deep_copy_shader_source_str)
    : Shader(init_deep_copy_shader_source_str),
      m_vertexShaderId(0),
      m_fragmentShaderId(0),
      m_programId(0)
{
    m_pRenderable = renderable;

    if (isInitDeepCopyShaderSourceStr() == true)
    {
        m_pVertexShaderSource = new std::string(*vs_source);
        m_pFragmentShaderSource = new std::string(*fs_source);
    }
    else
    {
        m_pVertexShaderSource = vs_source;
        m_pFragmentShaderSource = fs_source;
    }
}

GLSLShader::~GLSLShader()
{
    if (isInitDeepCopyShaderSourceStr() == true)
    {
        DiscardShaderSource();
    }

    Remove();
}

GLuint GLSLShader::getProgramHandle() const
{
    if (isCreateOK() == true)
    {
        return m_programId;
    }
    return -1;
}

void GLSLShader::LoadShader(GLuint shader_handle, const std::string& shader_source)
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

    //TODO, add GL_ERROR check for return value
    //maybe a class for GL_Utility?

    return result;
}

bool GLSLShader::Setup()
{
    bool result = true;

    LOGD("begin to call glUseProgram(%d)", m_programId);
    glUseProgram(m_programId);

    //TODO, add GL_ERROR check for return value
    //maybe a class for GL_Utility?

    return result;
}

bool GLSLShader::UnSetup()
{
    GLuint m_Invalid_Program = 0;
    glUseProgram(m_Invalid_Program);
    return true;
}


void GLSLShader::Remove()
{
    // TODO, remove the created shader handle, program id from system, for release system resource
}

void GLSLShader::DiscardShaderSource()
{
    if (isInitDeepCopyShaderSourceStr() == true)
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

GLInputVertexAttribute::GLInputVertexAttribute(std::string name, GLRenderable* renderable)
    : InputVertexAttribute(name),
      m_IAHandle(-1),
      m_IAElementNum(4),
      m_IAType(GL_FLOAT),
      m_IANormalized(GL_FALSE),
      m_IAStride(0),
      m_IAOffset(0) 
{
    m_pRenderable = renderable;
};

GLInputVertexAttribute::~GLInputVertexAttribute()
{
}

bool GLInputVertexAttribute::Create()
{
    if (isCreateOK() == false)
    {
        bool result = false;
        GLSLShader* shader = (dynamic_cast<GLRenderable*>(this->getRenderable()))->getShader();

        if (shader->isCreateOK())
        {
            LOGD("begin to call glGetAttribLocation(%d, %s)", shader->getProgramHandle(), m_InputVertexAttributeName.c_str());
            m_IAHandle = glGetAttribLocation(shader->getProgramHandle(), m_InputVertexAttributeName.c_str());
            result = m_IAHandle != -1 ? true : false;
        }

        m_bIsCreateOK = result;
    }

    return isCreateOK();
}

bool GLInputVertexAttribute::Enable()
{
    bool result = false;
    GLSLShader* shader = (dynamic_cast<GLRenderable*>(this->getRenderable()))->getShader();
    GLMesh* mesh = (dynamic_cast<GLRenderable*>(this->getRenderable()))->getMesh();

    if (shader->isCreateOK() && shader->isEnableOK() && mesh != NULL)
    {
        glVertexAttribPointer(m_IAHandle, m_IAElementNum, m_IAType, m_IANormalized, m_IAStride, (BYTE*)(mesh->getVertexDataPointer()) + m_IAOffset);
        glEnableVertexAttribArray(m_IAHandle);
        result = true;
    }

    m_bIsEnableOK = result;
    return isEnableOK();
}

bool GLInputVertexAttribute::Disable()
{
    bool result = true;

    glDisableVertexAttribArray(m_IAHandle);

    m_bIsEnableOK = !result;
    return result;
}

bool GLRenderable::Init()
{
    m_pGeometry->getMesh()->Create();
    m_pGeometry->getShader()->Create();
    std::vector<GLInputVertexAttribute*> m_InputAttributes = m_pGeometry->getAllInputVertexAttributes();

    for (std::vector<GLInputVertexAttribute*>::iterator i = m_InputAttributes.begin(); i != m_InputAttributes.end(); ++i)
    {
        (*i)->Create();
    }
    return true;
}

bool GLRenderable::Draw()
{
    m_pGeometry->getMesh()->Enable();
    m_pGeometry->getShader()->Enable();
    std::vector<GLInputVertexAttribute*> m_InputAttributes = m_pGeometry->getAllInputVertexAttributes();

    for (std::vector<GLInputVertexAttribute*>::iterator i = m_InputAttributes.begin(); i != m_InputAttributes.end(); ++i)
    {
        (*i)->Enable();
    }

    GLenum mode = m_pGeometry->getMesh()->getPrimitiveTopologe(); // primitive mode in GLenum
    GLsizei count = m_pGeometry->getMesh()->getNumOfIndices(); // number of Index
    GLenum type = m_pGeometry->getMesh()->getIndexType(); // type of one Index in GLenum
    GLvoid* indices = m_pGeometry->getMesh()->getIndexPointerOrOffset(); // pointer to index array in CPU or offset of index buffer's offset

    glDrawElements(mode, count, type, indices);

    return true;
}

bool GLRenderable::Destroy()
{
    m_pGeometry->getMesh()->Disable();
    m_pGeometry->getShader()->Disable();
    std::vector<GLInputVertexAttribute*> m_InputAttributes = m_pGeometry->getAllInputVertexAttributes();

    for (std::vector<GLInputVertexAttribute*>::iterator i = m_InputAttributes.begin(); i != m_InputAttributes.end(); ++i)
    {
        (*i)->Disable();
    }
    return true;
}

GLMesh* GLRenderable::getMesh() const
{
    return this->m_pGeometry->getMesh();
}

GLSLShader* GLRenderable::getShader() const
{
    return this->m_pGeometry->getShader();
}

GLInputVertexAttribute* GLRenderable::getInputVertexAttribute(std::string name) const
{
    return this->m_pGeometry->getInputVertexAttribute(name);
}

std::vector<GLInputVertexAttribute*> GLRenderable::getAllInputVertexAttributes() const
{
    return this->m_pGeometry->getAllInputVertexAttributes();
}

bool GLRenderable::setMesh(GLMesh* val)
{
    val->setRenderable(this);
    this->m_pGeometry->setMesh(val);
    return true;
}

bool GLRenderable::setShader(GLSLShader* val)
{
    val->setRenderable(this);
    this->m_pGeometry->setShader(val);
    return true;
}

bool GLRenderable::addInputVertexAttribute(GLInputVertexAttribute* val)
{
    val->setRenderable(this);
    this->m_pGeometry->addInputVertexAttribute(val);
    return true;
}

GLClearRenderable::GLClearRenderable()
    : m_fClearColorRed(1.0),
      m_fClearColorGreen(1.0),
      m_fClearColorBlue(1.0),
      m_fClearColorAlpha(1.0),
      m_fClearDepth(1.0),
      m_iClearStencil(0),
      m_bIsClearColor(true),
      m_bIsClearDepth(true),
      m_bIsClearStencil(false)
{
}

bool GLClearRenderable::Init()
{
    return true;
}

bool GLClearRenderable::Draw()
{
    GLbitfield mask = 0;
    if (m_bIsClearColor)
    {
        mask |= GL_COLOR_BUFFER_BIT;
    }
    if (m_bIsClearDepth)
    {
        mask |= GL_DEPTH_BUFFER_BIT;
    }
    if (m_bIsClearStencil)
    {
        mask |= GL_STENCIL_BUFFER_BIT;
    }

    glClearColor(m_fClearColorRed, m_fClearColorGreen, m_fClearColorBlue, m_fClearColorAlpha);
    glClear(mask);
    return true;
}

bool GLClearRenderable::Destroy()
{
    return true;
}