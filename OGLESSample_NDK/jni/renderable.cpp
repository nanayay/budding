#include "renderable.h"

Shader::Shader(bool init_deep_copy_shader_source_str)
    :m_isInitDeepCopyShaderSourceStr(init_deep_copy_shader_source_str),
     m_isShaderCreateOK(false),
     m_isShaderEnableOK(false)
{
}

Shader::~Shader()
{
    // Notice, destructor can not call any kind of pure virtual function, and better not call virtual function
    /*
    if (isInitDeepCopyShaderSourceStr() == true)
    {
        DiscardShaderSource();
    }

    Remove();
    */
}

bool Shader::Create()
{
    if (isCreateOK() == false)
    {
        bool result = CompileAndLink();
        if (isInitDeepCopyShaderSourceStr() == true)
        {
            if (result == true)
            {
                DiscardShaderSource();
            }
            else
            {
                // Do nothing when compiler error, but this class hold the deep copy for shader source
                // Here discard shader source is the de-constructor's job
            }
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

bool Shader::Disable()
{
    bool result = UnSetup();
    m_isShaderEnableOK = !result;
    return result;
}

Geometry::Geometry(const std::string& name, unsigned int vertex_number, unsigned int index_number, size_t one_vertex_size, size_t one_index_size, size_t vertex_stride, const void* vertex_buffer_data, const void* index_buffer_data, unsigned int va_position_elem_num)
    : m_name(name),
      m_numVertices(vertex_number),
      m_numIndices(index_number),
      m_sizeOneVertexInByte(one_vertex_size),
      m_sizeOneIndexInByte(one_index_size),
      m_vertexStride(vertex_stride),
      m_pVertices(vertex_buffer_data),
      m_pIndices(index_buffer_data),
      m_numVAPositionElements(va_position_elem_num)
{
}

Geometry::~Geometry()
{
}
