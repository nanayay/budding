#include "gl_renderable.h"

GLMesh::GLMesh(bool use_cpu_buffer, GLRenderable* renderable)
    : MeshRawData(),
      m_bIsUseCPUBuffer(use_cpu_buffer)
{
    m_pRenderable= renderable;
}

GLMesh::~GLMesh()
{
    this->Disable();
    this->Dispose();
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

            // add glUnBindBuffer, add glDeleteBuffer to these
            // check glDisableVertexAttribArray() is called properly
        }
        result = true;

        m_bIsCreateOK = result;
    }
    return isCreateOK();
}

bool GLMesh::Enable()
{
    if (isCreateOK())
    {
        bool result = false;

        // Note, glBindBuffer for VBO will be called in IA's Enable, and IBO's bind will be called in GLMesh's Enable
        if (isUseCPUBuffer() == false)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        }
        result = true;

        m_bIsEnableOK = result;
    }
    return isEnableOK();
}

bool GLMesh::Disable()
{
    if (isCreateOK())
    {
        bool result = false;

        // Note, UnBind buffer for VBO will be called in IA's Disable, but IBO's unbind will be called in GLMesh's Disable

        if (isUseCPUBuffer() == false)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        result = true;

        m_bIsEnableOK = !result;
    }
    return !isEnableOK();
}

bool GLMesh::Dispose()
{
    if (isCreateOK())
    {
        bool result = false;

        GLuint vbos[] = {m_vbo, m_ibo};
        glDeleteBuffers(sizeof(vbos)/sizeof(vbos[0]), vbos);
        result = true;

        m_bIsCreateOK = !result;
    }
    return !isCreateOK();
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
    LOGD("glUseProgram(%d)", m_programId);

    return result;
}

bool GLSLShader::UnSetup()
{
    GLuint m_Invalid_Program = 0;
    glUseProgram(m_Invalid_Program);
    LOGD("glUseProgram(%d)", m_Invalid_Program);
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

bool GLSLShader::Create()
{
    if (isCreateOK() == false)
    {
        bool result = false;
        result = CompileAndLink();
        if (isInitDeepCopyShaderSourceStr() == true)
        {
            if (result == true)
            {
                DiscardShaderSource();
            }
        }
        m_bIsCreateOK = result;
    }
    return isCreateOK();
}

bool GLSLShader::Enable()
{
    if (isCreateOK())
    {
        bool result = false;
        result = Setup();
        m_bIsEnableOK = result;
    }
    return isEnableOK();
}

bool GLSLShader::Disable()
{
    if (isCreateOK())
    {
        bool result = false;
        result = UnSetup();
        m_bIsEnableOK = !result;
    }
    return !isEnableOK();
}

bool GLSLShader::Dispose()
{
    if (isCreateOK())
    {
        bool result = false;

        DiscardShaderSource();
        Remove();
        result = true;

        m_bIsCreateOK = !result;
    }
    return !isCreateOK();
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
    this->Disable();
    this->Dispose();
}

bool GLInputVertexAttribute::Create()
{
    // Note
    // Notebook here
    
    // Mesh's Create and Dispose will call glGenBuffers and glDeleteBuffers, and upload data to it
    // Mesh's Enable and Disable will do nothing
    // IA's Create and Dispose will do nothing
    // IA's Enable and Disable will call glGetAttribLocation, glEnableVertexAttribArray, glVertexAttribPointer, etc
    // Shader's Create and Dispose will call glCreateProgram, glDeleteProgram
    // Shader's Enable and Disable will call glUseProgram

    // Enable and Disable should can be called several times
    //     - Way 1: after call Enable() in Draw() of Renderable, call Disable() before out-of-scope [good OOP, but bad performance]
    //     - Way 2: just make Enable & Disable did not hold the flag for is_Enable_OK, call Enable will do what it have to do [good performance, good OOP] [Choose this one]
    // Create and Dispose should can not be called several times

    // Move glGetAttribLocation from Create to Enable, IA handle should be queried in real time
    if (isCreateOK() == false)
    {
        bool result = false;

        // Note, can not call glGetAttribLocation() here, since
        // - at the very beginning, IA is not bind to Renderable, hence can not get the corresponding Shader [otherwise, crash will happen due to null dereference]
        // - sometimes, we may bind several Shader with the same IA, we should not fix the handle at the IA create() time, but get the handle when it Enable() at the real time
        result = true;

        m_bIsCreateOK = result;
    }

    return isCreateOK();
}

bool GLInputVertexAttribute::Enable()
{
    if (isCreateOK())
    {
        bool result = false;

        GLSLShader* shader = (dynamic_cast<GLRenderable*>(this->getRenderable()))->getShader();
        GLMesh* mesh = (dynamic_cast<GLRenderable*>(this->getRenderable()))->getMesh();

        if (shader->isCreateOK() && shader->isEnableOK() && mesh != NULL)
        {

            m_IAHandle = glGetAttribLocation(shader->getProgramHandle(), m_InputVertexAttributeName.c_str());
            LOGD("call glGetAttribLocation(%d, %s), get the input attribute handle is %d", shader->getProgramHandle(), m_InputVertexAttributeName.c_str(), m_IAHandle);

            if (m_IAHandle != -1)
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
            else
            {
                LOGE("glGetAttribLocation return -1");
                result = false;
            }
        }
        m_bIsEnableOK = result;
    }
    return isEnableOK();
}

bool GLInputVertexAttribute::Disable()
{
    if (isCreateOK())
    {
        bool result = false;

        GLMesh* mesh = (dynamic_cast<GLRenderable*>(this->getRenderable()))->getMesh();
        glDisableVertexAttribArray(m_IAHandle);
        if (mesh->isUseCPUBuffer() == false)
        {
            // Unbind VBO
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        result = true;

        m_bIsEnableOK = !result;
    }
    return !isEnableOK();
}

bool GLInputVertexAttribute::Dispose()
{
    if (isCreateOK())
    {
        bool result = false;

        result = true;

        m_bIsCreateOK = !result;
    }
    return !isCreateOK();
}




// Renderable will also call ElementOfRenderable's Create and Dispose
// Scene will also do call ElementOfRenderable's Create and Dispose
// but ElementOfRenderable's Create and Dispose can be called several times
// Although, it is fine to call ElementOfRenderable's Create and Dispose in different places, in theory, Scene hold the lifecycle of ElementOfRenderable, Renderable just use it
// Enable() and Disable() of ElementOfRenderable will be re-call-able
//    - in Renderable's Draw() will no need to call Disable() before exit, that is a good cpu performance improvement
//    - Enable() and Disable() re-call-able will make the next Renderable's Draw() can over-write the current one's OGL states, in case they share the same IA, Mesh, or Shader

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
        glDrawElements(mode, count, type, offset);
    }

    // Note
    // You can call each Mesh, IA, Shader's Disable() here to unset the OGL stage, but that is not good performance when you have several Renderables need to be Draw()
    // So, not call ElementOfRenderable's Disable here, but just make Enable/Disable be re-call-able

    return true;
}

bool GLRenderable::Destroy()
{
    // Renderable did not hold the life cycle of ElementOfRenderable, but juse use them
    // Scene hold the life cycle of ElementOfRenderable
    // Although, ElementOfRenderable's Create() and Dispose() can be called several times, but it is not fine to call it in Renderable
    // Only GLScene's UnLoad() can call ElementOfRenderable's Dispose()
    // And, Renderable and GLScene's Load() can call ElementOfRenderable's Create()

    // GLRenderable's Destory() will be called in Renderer's Destory() or when GLScene's UnLoad() is called
    // Since Renderable did not hold the lifecycle of ElementOfRenderable, just Disable them when Destroy Renderable

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