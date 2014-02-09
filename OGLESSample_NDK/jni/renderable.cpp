#include "renderable.h"

Shader::Shader(bool discard_shader_source)
    :m_isShaderSourceDiscardable(discard_shader_source),
     m_isShaderCreateOK(false),
     m_isShaderEnableOK(false)
{
}

Shader::~Shader() {}

bool Shader::Create()
{
    if (isCreateOK() == false)
    {
        bool result = CompileAndLink();
        if (result == true && isShaderSourceDiscardable() == true)
        {
            DiscardShaderSource();
        }
        m_isShaderCreateOK = result;
    }
    return isCreateOK();
}

bool Shader::Enable()
{
    m_isShaderEnableOK = Setup();
    return isEnableOK();
}

Geometry::Geometry(std::string name, bool isShallowCopy)
    : m_name(name),
      m_numVertices(0),
      m_numIndices(0),
      m_sizeOneVertexInByte(0),
      m_sizeOneIndexInByte(0),
      m_pVertices(NULL),
      m_pIndices(NULL),
      m_numVAPositionElements(0),
      m_numVAColorElements(0),
      m_numVATexCoordElements(0),
      m_vertexStride(0),
      m_isShallowData(isShallowCopy)
{
}

Geometry::Geometry(std::string name, unsigned int vertex_number, unsigned int index_number, size_t one_vertex_size, size_t one_index_size, void* vertex_buffer_data, void* index_buffer_data, bool isShallowCopy)
    : m_name(name),
      m_numVertices(vertex_number),
      m_numIndices(index_number),
      m_sizeOneVertexInByte(one_vertex_size),
      m_sizeOneIndexInByte(one_index_size),
      m_pVertices(NULL),
      m_pIndices(NULL),
      m_numVAPositionElements(0),
      m_numVAColorElements(0),
      m_numVATexCoordElements(0),
      m_vertexStride(0),
      m_isShallowData(isShallowCopy)
{
    if (isShallowCopy == true)
    {
        // shallow copy means the Geometry class didn't have the vertex and index buffer data's life cycle
        // that means, Geometry will not have the right to release these memory space 
        m_pVertices = vertex_buffer_data;
        m_pIndices = index_buffer_data;
    }
    else
    {
        //TODO, make the data is deep copy but not the shallow data
    }
}

Geometry::~Geometry()
{
    if (m_isShallowData == false)
    {
        // TODO
    }
}

void setVertexBufferData(void* buffer, size_t one_vertex_size)
{
    // TODO, 移除set one vertex size等函数, 所有的都在这里设置?
    // 是的, 不提供其他的设置one vertex size, num vertex的选项, 一个buffer应该只对应一个geometry class, 且一个buffer也应该只有一个one vertex size和num vertex的属性
    // TODO, 根据shallow copy的属性, 来决定如何维护数据
    m_pVertices = buffer;
}

void setIndexBufferData(void* buffer, size_t one_index_size)
{
    // TODO 
    m_pIndices = buffer;
}


