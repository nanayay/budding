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

protected:
    virtual bool BindElementOfRenderableToMe() = 0;

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
    virtual bool Dispose() = 0;

    // getter
    bool isCreateOK() const { return m_bIsCreateOK; }
    bool isEnableOK() const { return m_bIsEnableOK; }

public:
    // setter and getter
    Renderable* getRenderable() const { return m_pRenderable; }
    void setRenderable(Renderable* val) { m_pRenderable = val; }

protected:
    Renderable* m_pRenderable;
    bool m_bIsCreateOK;
    bool m_bIsEnableOK;

private:
    ElementOfRenderable(ElementOfRenderable& copy_);
    ElementOfRenderable(const ElementOfRenderable& copy_);
    ElementOfRenderable& operator=(ElementOfRenderable& assign_);
    ElementOfRenderable& operator=(const ElementOfRenderable& assign_);

};

// Class
class Shader
    : public ElementOfRenderable
{
public:
    explicit Shader(bool init_deep_copy_shader_source_str = false);
    virtual ~Shader();

    virtual bool Create() = 0;
    virtual bool Enable() = 0;
    virtual bool Disable() = 0;
    virtual bool Dispose() = 0;

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
    explicit InputVertexAttribute(std::string name)
        : m_InputVertexAttributeName(name)
    {};
    virtual ~InputVertexAttribute() {};

    const std::string& getName() const { return m_InputVertexAttributeName; }

    bool operator==(const InputVertexAttribute& val) const { return m_InputVertexAttributeName == val.getName(); }

    // todo notebook, operator() is needed by find_if and remove_if, but the key here is when you use *val to find_if, it will have a temp object of current call, hence, here is the rule, not use the resource class as a function object
    
    /*
    bool operator()(const InputVertexAttribute* val) const
    {
        return m_InputVertexAttributeName == val->getName();
    }
    */

protected:
    std::string m_InputVertexAttributeName;
};

class MeshRawData
    : public ElementOfRenderable
{
public:
    MeshRawData() : m_IndicesDataOffset(0)
    {};
    virtual ~MeshRawData() {};

    // Always do Shallow copy, never Deep copy for saving memory footprint
    bool isDeepCopyData() const { return false; }

    // necessary information
    void*  getVertexDataPointer() const { return m_pVerticesDataPointer; }
    size_t getVertexDataSize() const { return m_sizeOfVerticesData; }
    void*  getIndexDataPointer() const { return m_pIndicesDataPointer; }
    size_t getIndexDataSize() const { return m_sizeOfIndicesData; }
    unsigned int getNumOfIndices() const { return m_numOfIndices; }
    size_t getIndexDataOffset() const { return m_IndicesDataOffset; }

    // maybe redundant information
    //unsigned int getNumOfVertices() const { return m_numOfVertices; }
    //size_t getOneVertexSize() const { return m_sizeofOneVertex; }
    //size_t getOneIndexSize() const { return m_sizeofOneIndex; }

    // necessary information
    void setVertexDataPointer(void* p) { m_pVerticesDataPointer = p; }
    void setVertexDataSize(size_t s) { m_sizeOfVerticesData = s; }
    void setIndexDataPointer(void* p) { m_pIndicesDataPointer = p; }
    void setIndexDataSize(size_t s) { m_sizeOfIndicesData = s; }
    void setNumOfIndices(unsigned int num) { m_numOfIndices = num; }
    void setIndexDataOffset(size_t offset) { m_IndicesDataOffset = offset; }

    // maybe redundant information
    //void setNumOfVertices(unsigned int num) { m_numOfVertices = num; }
    //void setOneVertexSize(size_t size) { m_sizeofOneVertex = size; }
    //void setOneIndexSize(size_t size) { m_sizeofOneIndex = size; }

private:
    // necessary information
    void* m_pVerticesDataPointer;
    size_t m_sizeOfVerticesData;

    void* m_pIndicesDataPointer;
    size_t m_sizeOfIndicesData;

    unsigned int m_numOfIndices;
    size_t m_IndicesDataOffset;

    // maybe redundant information
    // unsigned int m_numOfVertices;
    // size_t m_sizeofOneVertex;
    // size_t m_sizeofOneIndex;

public:
    //friend class Geometry;
    // TODO Question? friend of is only for Geometry or about Geometry's subclass?
    // TODO What about when Geometry is a template class?
};

class TextureSampler
    : public ElementOfRenderable
{
public:
    explicit TextureSampler() {};
    virtual ~TextureSampler() {};
};

class Texture
    :public ElementOfRenderable
{
public:
    explicit Texture(const std::string& texture_name, TextureSampler* texture_sampler = NULL)
        : m_nameID(texture_name),
          m_pTextureSampler(texture_sampler)
    {};
    virtual ~Texture() {};

public:
    const std::string& getName() const { return m_nameID; }
    // todo notebook, when use below will be some compile error
    // use const std::string& or just std::string as return type
    // std::string& getName() const { return m_nameID; }

    bool operator==(const Texture& val) const { return m_nameID == val.getName(); }

    // todo notebook, operator() is needed by find_if and remove_if, but the key here is when you use *val to find_if, it will have a temp object of current call, hence, here is the rule, not use the resource class as a function object

    /*
    bool operator()(const Texture* val) const { return m_nameID == val->getName(); }
    */

    void setTextureSampler(TextureSampler* val) { m_pTextureSampler = val; }

protected:
    std::string m_nameID;
    TextureSampler* m_pTextureSampler;
};

template <typename CLASS>
class CompareName
{
public:
    explicit CompareName(const std::string& val) : m_val(val) {};
    virtual ~CompareName() {};

    bool operator()(const CLASS* val) { return val->getName() == m_val; }

private:
    std::string m_val;

};

// Geometry is more like a container than a normal class object, use template to hold the shader, input vertex attribute, mesh data for general manager
template <typename MESH, typename SHADER, typename SAMPLER, typename TEXTURE, typename INPUTATTRIBUTE>
class Geometry
{
public:
    explicit Geometry(const std::string& name) : m_name(name), m_pSampler(NULL) {};
    virtual ~Geometry() {};

    std::string& getName() const { return m_name; }

    // Always do Shallow copy, never Deep copy for saving memory for saving memory footprint
    bool isDeepCopyData() { return false; }

    // getter
    MESH* getMesh() const { return m_pMesh; }
    SHADER* getShader() const { return m_pShader; }
    SAMPLER* getSampler() const { return m_pSampler; }
    std::vector<INPUTATTRIBUTE*> getAllInputVertexAttributes() const { return m_inputVertexAttributesVector; }
    INPUTATTRIBUTE* getInputVertexAttribute(std::string name) const
    {
        // vector will hold the pointer to IVA, hence you can not use find for the pointer, but only find_if for some user-defined pointer reference
        typename std::vector<INPUTATTRIBUTE*>::const_iterator i = std::find_if(m_inputVertexAttributesVector.begin(), m_inputVertexAttributesVector.end(), CompareName<INPUTATTRIBUTE>(name));
        return i != m_inputVertexAttributesVector.end() ? *i : NULL;
    }
    std::vector<TEXTURE*> getAllTextures() const { return m_textureVector; }
    TEXTURE* getTexture(std::string name) const
    {
        typename std::vector<TEXTURE*>::const_iterator i = std::find_if(m_textureVector.begin(), m_textureVector.end(), CompareName<TEXTURE>(name));
        return i != m_textureVector.end() ? *i : NULL;
    }

    // setter
    void setMesh(MESH* val) { m_pMesh = val; }
    void setShader(SHADER* val) { m_pShader = val; }
    void setSampler(SAMPLER* val) { m_pSampler = val; }
    void setAllInputVertexAttributes(std::vector<INPUTATTRIBUTE*> val)
    { 
        m_inputVertexAttributesVector = val;
    }
    void addInputVertexAttribute(INPUTATTRIBUTE* val)
    {
        typename std::vector<INPUTATTRIBUTE*>::iterator last = std::remove_if(m_inputVertexAttributesVector.begin(), m_inputVertexAttributesVector.end(), CompareName<INPUTATTRIBUTE>(val->getName()));
        if (last < m_inputVertexAttributesVector.end())
        {
            m_inputVertexAttributesVector.erase(last, m_inputVertexAttributesVector.end());
        }
        m_inputVertexAttributesVector.push_back(val);
    }
    void setAllTextures(std::vector<TEXTURE*> val)
    {
        m_textureVector = val;
    }
    void addTexture(TEXTURE* val)
    {
        typename std::vector<TEXTURE*>::iterator last = std::remove_if(m_textureVector.begin(), m_textureVector.end(), CompareName<TEXTURE>(val->getName()));
        if (last < m_textureVector.end())
        {
            m_textureVector.erase(last, m_textureVector.end());
        }
        m_textureVector.push_back(val);
    }

private:
    std::string m_name;

private:
    MESH*   m_pMesh;
    SHADER* m_pShader;
    SAMPLER* m_pSampler;
    std::vector<TEXTURE*> m_textureVector;
    std::vector<INPUTATTRIBUTE*> m_inputVertexAttributesVector;

};

#endif