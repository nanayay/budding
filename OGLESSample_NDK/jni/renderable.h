#ifndef AY_RENDERABLE
#define AY_RENDERABLE

#include <string>
#include <vector>
#include <algorithm>

// Renderable's element have same interfaces: Create(), Enable(), Disable()
// Renderable have same interfaces: Init(), Draw(), Destroy()

// Interface
class Renderable
{
public:
    Renderable() {};
    virtual ~Renderable() {};

    virtual bool Init() = 0;
    virtual bool Draw() = 0;
    virtual bool Destroy() = 0;

};

class ElementOfRenderable
{
public:
    explicit ElementOfRenderable(Renderable* val = NULL)
        : m_pRenderable(val),
          m_bIsCreateOK(false),
          m_bIsEnableOK(false)
    {};
    virtual ~ElementOfRenderable() {};

    virtual bool Create() = 0;
    virtual bool Enable() = 0;
    virtual bool Disable() = 0;

    // getter
    bool isCreateOK() const { return m_bIsCreateOK; }
    bool isEnableOK() const { return m_bIsEnableOK; }

protected:
    // setter and getter
    Renderable* getRenderable() const { return m_pRenderable; }
    void setRenderable(Renderable* val) { m_pRenderable = val; }

protected:
    Renderable* m_pRenderable;
    bool m_bIsCreateOK;
    bool m_bIsEnableOK;

};

// Class
class Shader
    : public ElementOfRenderable
{
public:
    explicit Shader(bool init_deep_copy_shader_source_str = false);
    virtual ~Shader();

    virtual bool Create();
    virtual bool Enable();
    virtual bool Disable();

public:
    bool isInitDeepCopyShaderSourceStr() const { return m_isInitDeepCopyShaderSourceStr; }

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

};

class InputVertexAttribute
    : public ElementOfRenderable
{
public:
    explicit InputVertexAttribute(std::string name) {};
    virtual ~InputVertexAttribute() {};

    std::string getName() const { return m_InputVertexAttributeName; }

    bool operator==(const InputVertexAttribute& val) const { return m_InputVertexAttributeName == val.getName(); }
    bool operator()(const InputVertexAttribute* val) const
    {
        return m_InputVertexAttributeName == val->getName();
    }

protected:
    std::string m_InputVertexAttributeName;
};

class MeshRawData
    : public ElementOfRenderable
{
public:
    MeshRawData() {};
    virtual ~MeshRawData() {};

    // Always do Shallow copy, never Deep copy for saving memory footprint
    bool isDeepCopyData() const { return false; }

    // necessary information
    void*  getVertexDataPointer() const { return m_pVerticesDataPointer; }
    size_t getVertexDataSizeInByte() const { return m_sizeOfVerticesData; }
    void*  getIndexDataPointer() const { return m_pIndicesDataPointer; }
    size_t getIndexDataSizeInByte() const { return m_sizeOfIndicesData; }
    unsigned int getNumOfIndices() const { return m_numOfIndices; }

    // maybe redundant information
    //unsigned int getNumOfVertices() const { return m_numOfVertices; }
    //size_t getOneVertexSizeInByte() const { return m_sizeofOneVertexInByte; }
    //size_t getOneIndexSizeInByte() const { return m_sizeofOneIndexInByte; }

    // necessary information
    void  setVertexDataPointer(void* p) { m_pVerticesDataPointer = p; }
    void  setVertexDataSizeInByte(size_t s) { m_sizeOfVerticesData = s; }
    void  setIndexDataPointer(void* p) { m_pIndicesDataPointer = p; }
    void  setIndexDataSizeInByte(size_t s) { m_sizeOfIndicesData = s; }
    void setNumOfIndices(unsigned int num) { m_numOfIndices = num; }

    // maybe redundant information
    //void setNumOfVertices(unsigned int num) { m_numOfVertices = num; }
    //void setOneVertexSizeInByte(size_t size) { m_sizeofOneVertexInByte = size; }
    //void setOneIndexSizeInByte(size_t size) { m_sizeofOneIndexInByte = size; }

private:
    // necessary information
    void* m_pVerticesDataPointer;
    size_t m_sizeOfVerticesData;

    void* m_pIndicesDataPointer;
    size_t m_sizeOfIndicesData;

    unsigned int m_numOfIndices;

    // maybe redundant information
    // unsigned int m_numOfVertices;
    // size_t m_sizeofOneVertexInByte;
    // size_t m_sizeofOneIndexInByte;

public:
    //friend class Geometry;
    // TODO Question? friend of is only for Geometry or about Geometry's subclass?
    // TODO What about when Geometry is a template class?
};

// Geometry is more like a container than a normal class object, use template to hold the shader, input vertex attribute, mesh data for general manager
template <typename MESH, typename SHADER, typename INPUTATTRIBUTE>
class Geometry
{
public:
    explicit Geometry(const std::string& name) : m_name(name) {};
    virtual ~Geometry() {};

    std::string getName() const { return m_name; }

    // Always do Shallow copy, never Deep copy for saving memory for saving memory footprint
    bool isDeepCopyData() { return false; }

    // getter
    MESH* getMesh() const { return m_pMesh; }
    SHADER* getShader() const { return m_pShader; }
    std::vector<INPUTATTRIBUTE*> getAllInputVertexAttributes() const { return m_inputVertexAttributesVector; }
    INPUTATTRIBUTE* getInputVertexAttribute(std::string name) const
    {
        // vector will hold the pointer to IVA, hence you can not use find for the pointer, but only find_if for some user-defined pointer reference
        typename std::vector<INPUTATTRIBUTE*>::const_iterator i = std::find_if(m_inputVertexAttributesVector.begin(), m_inputVertexAttributesVector.end(), INPUTATTRIBUTE(name));
        return i != m_inputVertexAttributesVector.end() ? *i : NULL;
    }

    // setter
    void setMesh(MESH* val) { m_pMesh = val; }
    void setShader(SHADER* val) { m_pShader = val; }
    void setAllInputVertexAttributes(std::vector<INPUTATTRIBUTE*> val) { m_inputVertexAttributesVector = val; }
    void addInputVertexAttribute(INPUTATTRIBUTE* val)
    {
        typename std::vector<INPUTATTRIBUTE*>::iterator last = std::remove_if(m_inputVertexAttributesVector.begin(), m_inputVertexAttributesVector.end(), *val);
        m_inputVertexAttributesVector.erase(last, m_inputVertexAttributesVector.end());
        m_inputVertexAttributesVector.push_back(val);
    }

private:
    std::string m_name;

private:
    MESH*   m_pMesh;
    SHADER* m_pShader;
    std::vector<INPUTATTRIBUTE*> m_inputVertexAttributesVector;

};

#endif