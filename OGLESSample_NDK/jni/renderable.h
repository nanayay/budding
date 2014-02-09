#ifndef AY_RENDERABLE
#define AY_RENDERABLE

class Shader
{
public:
    explicit Shader(bool discard_shader_source = false);
    virtual ~Shader();

    // Create() will not allow force re-create smoothly built shader, that is, if the shader already compile and link smoothly and OK, there should be no way for user to re-build this; but if the shader is error and compile and link error, then user can ignore the DiscardShaderSource(), and re-build the shader
    // DiscardShaderSource() only happen when you set the flag and the shader compiled smoothly and OK
    bool Create();

    // Enable allow user to re-enable the shader, for different render pass
    bool Enable();

public:
    bool isShaderSourceDiscardable() const { return m_isShaderSourceDiscardable; }
    bool isCreateOK() const { return m_isShaderCreateOK; }
    bool isEnableOK() const { return m_isShaderEnableOK; }

protected:
    // Build shader source code
    virtual bool CompileAndLink() = 0;

    // Enable the shader to the pipeline 
    virtual bool Setup() = 0;

    // Delete the shader handle or resource from the system
    // Note, it did not like the logic to un-setup the shader to pipeline, just delete the shader
    // For the un-setup logic, it is better to hold in some OGL classes, not the shader class
    virtual void Remove() = 0;

    // Discard the shader source string hold in this class for more effective memory usage
    virtual void DiscardShaderSource() = 0;

protected:
    bool m_isShaderSourceDiscardable;
    bool m_isShaderCreateOK;
    bool m_isShaderEnableOK;

};

class Geometry
{
public:
    Geometry(std::string name);
    virtual ~Geometry();

    std::string getName() const { return m_name; }
    void setName(std::string name) { m_name = name; }

    unsigned int getNumVertices() const { return m_numVertices; }
    void setNumVertices(unsigned int num) { m_numVertices = num; }

    unsigned int getNumIndices() const { return m_numIndices; }
    void setNumIndices(unsigned int num) { m_numIndices = num; }

    size_t getOneVertexSizeInByte() const { return m_sizeOneVertexInByte; }
    void setOneVertexSizeInByte(size_t size) { m_sizeOneVertexInByte = size; }

    size_t getOneIndexSizeInByte() const { return m_sizeOneIndexInByte; }
    void setOneIndexSizeInByte(size_t size) { m_sizeOneIndexInByte = size; }

    void* getVertexBufferData() const { return m_pVertices; }
    void setVertexBufferData(void* buffer, size_t one_vertex_size);

    void* getIndexBufferData() const { return m_pIndices; }
    void setIndexBufferData(void* buffer, size_t one_index_size);

    unsigned int getNumVAPositionElements() { return m_numVAPositionElements; }
    void setNumVAPositionElements(unsigned int num) { m_numVAPositionElements = num; }

    unsigned int getNumVAColorElements() { return m_numVAColorElements; }
    void setNumVAColorElements(unsigned int num) { m_numVAColorElements = num; }

    unsigned int getNumVATexCoordElements() { return m_numVATexCoordElements; }
    void setNumVATexCoordElements(unsigned int num) { m_numVATexCoordElements = num; }

    unsigned int getVertexStride() { return m_vertexStride; }
    void setVertexStride(unsigned int num) { m_vertexStride = num; }

private:
    std::string m_name;
    unsigned int m_numVertices;
    unsigned int m_numIndices;
    size_t m_sizeOneVertexInByte;
    size_t m_sizeOneIndexInByte;
    void* m_pVertices;
    void* m_pIndices;

    // VA means Vertex Attribute
    unsigned int m_numVAPositionElements;
    unsigned int m_numVAColorElements;
    unsigned int m_numVATexCoordElements;

    unsigned int m_vertexStride;

    // m_isShallowData mean the vertex and index data pointer are allocated by the outside, and Geometry class didn't keep the life cycle of that memory
    bool m_isShallowData;
};

#endif