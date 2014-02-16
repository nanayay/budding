#ifndef AY_RENDERABLE
#define AY_RENDERABLE

#include <string>

class Shader
{
public:
    explicit Shader(bool init_deep_copy_shader_source_str = true);
    virtual ~Shader();

    // Create() not allow force re-create smoothly built shader, that is, if the shader already compile and link smoothly and OK, there should be no way for user to re-build this;
    // If the shader is error and compile and link error, User can re-create the shader, you can re-call Create(), but it is better to assert and stop program for some debug
    bool Create();

    // Enable() allow user to re-enable the shader, for different render pass
    bool Enable();

    // Disable() allow user to disable or un-setup the shader, for something need to be un-setup from pipeline
    bool Disable();

public:
    bool isInitDeepCopyShaderSourceStr() const { return m_isInitDeepCopyShaderSourceStr; }
    bool isCreateOK() const { return m_isShaderCreateOK; }
    bool isEnableOK() const { return m_isShaderEnableOK; }

protected:
    // Build shader source code
    virtual bool CompileAndLink() = 0;

    // Enable and Setup the shader to the pipeline 
    virtual bool Setup() = 0;

    // Disable and Un-setup the shader from the pipeline
    virtual bool UnSetup() = 0;

    // Delete the shader handle or resource from the system
    // Note, it did not like the logic to un-setup the shader to pipeline, just delete the shader
    virtual void Remove() = 0;

    // Delete the shader source string which deep copied to Shader class
    virtual void DiscardShaderSource() = 0;

protected:
    bool m_isInitDeepCopyShaderSourceStr;
    bool m_isShaderCreateOK;
    bool m_isShaderEnableOK;

};

class Geometry
{
public:
    Geometry(const std::string& name, unsigned int vertex_number, unsigned int index_number, size_t one_vertex_size, size_t one_index_size, size_t vertex_stride, const void* vertex_buffer_data, const void* index_buffer_data);
    virtual ~Geometry();

    // Always do Shallow copy, never Deep copy for saving memory for saving memory footprint
    bool isDeepCopyData() { return false; }

    std::string getName() const { return m_name; }
    unsigned int getNumVertices() const { return m_numVertices; }
    unsigned int getNumIndices() const { return m_numIndices; }
    size_t getOneVertexSizeInByte() const { return m_sizeOneVertexInByte; }
    size_t getOneIndexSizeInByte() const { return m_sizeOneIndexInByte; }
    size_t getVertexStride() const { return m_vertexStride; }
    const void* getVertexBufferData() const { return m_pVertices; }
    const void* getIndexBufferData() const { return m_pIndices; }

    unsigned int getNumVAPositionElements() const { return m_numVAPositionElements; }
    void setNumVAPositionElements(unsigned int num) { m_numVAPositionElements = num; }

    unsigned int getNumVAColorElements() const { return m_numVAColorElements; }
    void setNumVAColorElements(unsigned int num) { m_numVAColorElements = num; }

    unsigned int getNumVATexCoordElements() const { return m_numVATexCoordElements; }
    void setNumVATexCoordElements(unsigned int num) { m_numVATexCoordElements = num; }

private:
    std::string m_name;
    unsigned int m_numVertices;
    unsigned int m_numIndices;
    size_t m_sizeOneVertexInByte;
    size_t m_sizeOneIndexInByte;
    size_t m_vertexStride;
    const void* m_pVertices;
    const void* m_pIndices;

    // VA means Vertex Attribute
    unsigned int m_numVAPositionElements;
    unsigned int m_numVAColorElements;
    unsigned int m_numVATexCoordElements;

};

#endif