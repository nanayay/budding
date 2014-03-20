#include "type_defines.h"
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
            // index array buffer's cpu memory pointer no need to be set, it will get it directly when draw
        }
        else
        {
            // use vertex and index buffer in GPU memory
            GLuint vbos[2] = {0, 0};
            glGenBuffers(2, vbos);

            // vertex buffer object create
            m_vbo = vbos[0];
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, this->getVertexDataSize(), this->getVertexDataPointer(), GL_STATIC_DRAW);

            // index buffer object create
            m_ibo = vbos[1];
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexDataSize(), this->getIndexDataPointer(), GL_STATIC_DRAW);

            // TODO here
            // make these glBindBuffer to unbind when tear down
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

GLuint GLSLShader::LoadShader(GLenum shader_type, const std::string& shader_source)
{
    GLuint shader_handle = glCreateShader(shader_type);

    if (shader_handle)
    {
        const unsigned int NUM_SHADERS = 1;
        const GLchar* pSourceCode = shader_source.c_str();
        GLint length = shader_source.length();

        glShaderSource(shader_handle, NUM_SHADERS, &pSourceCode, &length);
        glCompileShader(shader_handle);

        GLint shader_compiled = 0;
        glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &shader_compiled);
        if (!shader_compiled)
        {
            GLint length = 0;
            glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &length);
            if (length)
            {
                std::string log(static_cast<size_t>(length), ' ');
                glGetShaderInfoLog(shader_handle, length, NULL, &log[0]);

                std::string shader_type_str;
                if (shader_type == GL_VERTEX_SHADER)
                {
                    shader_type_str = std::string("Vertex Shader");
                }
                else if (shader_type == GL_FRAGMENT_SHADER)
                {
                    shader_type_str = std::string("Fragment Shader");
                }
                else
                {
                    shader_type_str = std::string("Unknown Shader");
                }

                LOGE("Could not compile shader %d [%s]:\n%s\n", shader_type, shader_type_str.c_str(), log.c_str());
                glDeleteShader(shader_handle);
                shader_handle = 0;
            }
        }
    }

    return shader_handle;
}

GLuint GLSLShader::CreateProgram(const char* vertex_source, const char* fragment_source)
{
    GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, vertex_source);
    if (vertex_shader == 0)
        return 0;

    GLuint pixel_shader = LoadShader(GL_FRAGMENT_SHADER, fragment_source);
    if (pixel_shader == 0)
        return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, pixel_shader);
    glLinkProgram(program);

    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
    {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length)
        {
            std::string log(static_cast<size_t>(length), ' ');
            glGetProgramInfoLog(program, length, NULL, &log[0]);
            LOGE("Could not link program:\n%s\n", log.c_str());
        }
        glDeleteProgram(program);
        program = 0;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(pixel_shader);

    return program;
}

bool GLSLShader::CompileAndLink()
{
    m_programId = CreateProgram(m_pVertexShaderSource->c_str(), m_pFragmentShaderSource->c_str());

    LOGE("GLSL program handle create: %d", m_programId);

    return m_programId == 0 ? false : true;
}

bool GLSLShader::Setup()
{
    bool result = true;

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
    glDeleteProgram(m_programId);
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
            LOGD("get the input attribute handle for %s is %d", m_InputVertexAttributeName.c_str(), m_IAHandle);
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
        glEnableVertexAttribArray(m_IAHandle);

        if (mesh->isUseCPUBuffer())
        {
            // use cpu memory's vertex buffer pointer
            glVertexAttribPointer(m_IAHandle, m_IAElementNum, m_IAType, m_IANormalized, m_IAStride, (BYTE*)(mesh->getVertexDataPointer()) + m_IAOffset);
        }
        else
        {
            // use gpu memory's vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBOHandle());
            glVertexAttribPointer(m_IAHandle, m_IAElementNum, m_IAType, m_IANormalized, m_IAStride, (BYTE*)m_IAOffset);
        }
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
    GLenum type = m_pGeometry->getMesh()->getIndexDataType(); // type of one Index in GLenum

    if (m_pGeometry->getMesh()->isUseCPUBuffer())
    {
        GLvoid* indices = (BYTE*)(m_pGeometry->getMesh()->getIndexDataPointer()) + m_pGeometry->getMesh()->getIndexDataOffset(); // pointer to index array pointer or offset
        glDrawElements(mode, count, type, indices);
    }
    else
    {
        GLvoid* offset = (BYTE*)(m_pGeometry->getMesh()->getIndexDataOffset());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pGeometry->getMesh()->getIBOHandle());
        glDrawElements(mode, count, type, offset);
    }


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