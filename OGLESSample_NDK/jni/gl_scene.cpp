#include <algorithm>
#include "gl_scene.h"

// GL Scene will not keep class variable for each Renderable [GLClearRenderable(), GLRenderable("Rectangle"), etc], but just new it in its Load() function, and append to its Renderable Vector Pointer, delete them in UnLoad()
// Destructor of GL Scene will also call UnLoad() in case it's forgot

namespace Models
{

    // Redundant variables
    // Note, Num of Vertex is not necessary value for renderer
    unsigned int num_of_vertex = 4;

    // Necessary variables
    float vertes [] =
    {
        -0.5, 0.5, 0.0, // Pos 1
        1.0, 0.0, 0.0, 1.0, // Color 1

        0.5, 0.5, 0.0, // Pos 2
        0.0, 1.0, 0.0, 1.0, // Color 2

        -0.5, -0.5, 0.0, // Pos 3
        0.0, 0.0, 1.0, 1.0, // Color 3

        0.5, -0.5, 0.0, // Pos 4
        1.0, 1.0, 1.0, 1.0 // Color 4
    };

    unsigned int stride = sizeof(vertes)/num_of_vertex;
    unsigned int num_of_element_in_pos_for_one_vertex = 3;
    unsigned int num_of_element_in_color_for_one_vertex = 4;

    size_t offset_of_pos = 0;
    size_t offset_of_color = num_of_element_in_pos_for_one_vertex * sizeof(float);

    unsigned short indices [] =
    {
        0, 2, 1,
        2, 3, 1
    };
    unsigned int num_of_index = sizeof(indices)/sizeof(indices[0]);

    // TODO, add some logic to parser a string and generate the IA array
    std::string ia_pos("a_vPosition");
    std::string ia_color("a_vColor");

    const char* vs = 
        "#version 100\n"
        "attribute vec4 a_vPosition;\n"
        "attribute vec4 a_vColor;\n"
        "varying vec4 v_vColor;\n"
        "void main()\n"
        "{\n"
            "gl_Position = a_vPosition;\n"
            "v_vColor = a_vColor;\n"
        "}\n";

    const char* fs =
        "#version 100\n"
        //"precision lowp float;\n"
        //"precision mediump float;\n"
        "precision highp float;\n"
        "varying vec4 v_vColor;\n"
        "void main()\n"
        "{\n"
            "gl_FragColor = v_vColor;\n"
        "}\n";

    std::string vss(vs);
    std::string fss(fs);

    GLMesh* m_pGLMesh;
    GLSLShader* m_pGLSL;
    GLInputVertexAttribute* m_IAPos;
    GLInputVertexAttribute* m_IAColor;

    bool Import()
    {
        // TODO,
        // Memory leak for ElementOfRenderable? leave it??
        m_pGLMesh = new GLMesh(true);
        m_pGLSL = new GLSLShader(&vss, &fss);
        m_IAPos = new GLInputVertexAttribute(ia_pos);
        m_IAColor = new GLInputVertexAttribute(ia_color);

        // TODO Here, make the GLSLShader can also accept the char* as input
        //GLSLShader* m_pGLSL = new GLSLShader(vs, fs);
        // TODO Here, the GLSL's pointer will be undefined if out-of-scope

        m_pGLMesh->setVertexDataPointer(vertes);
        m_pGLMesh->setVertexDataSizeInByte(sizeof(vertes));
        m_pGLMesh->setIndexDataPointer(indices);
        m_pGLMesh->setIndexDataSizeInByte(sizeof(indices));
        m_pGLMesh->setNumOfIndices(num_of_index);

        // TODO Here, make these be default and in the constructor
        m_pGLMesh->setPrimitiveTopologe(GL_TRIANGLES);
        m_pGLMesh->setIndexType(GL_UNSIGNED_SHORT);

        // TODO Here, make these value be default
        // TODO Here, make these value be another enum which is easy to remember
        m_IAPos->setNormalized(false);
        m_IAPos->setStride(stride);
        m_IAPos->setType(GL_FLOAT);
        m_IAPos->setElementNum(num_of_element_in_pos_for_one_vertex);
        m_IAPos->setOffset(offset_of_pos);
        
        m_IAColor->setNormalized(false);
        m_IAColor->setStride(stride);
        m_IAColor->setType(GL_FLOAT);
        m_IAColor->setElementNum(num_of_element_in_color_for_one_vertex);
        m_IAColor->setOffset(offset_of_color);

        return true;
    }
};

bool GLBasicScene::Load()
{
    // Clear the Color and Depth buffer
    GLClearRenderable* m_pGlClear = new GLClearRenderable();

    m_pGlClear->setClearColorRGB(1.0, 1.0, 1.0, 1.0);

    m_pRenderablesVector->push_back(m_pGlClear);

    // Draw a Rectangle
    GLRenderable* m_pGLRect = new GLRenderable("Rectangle");

    // TODO, notebook
    // 设计的时候, 既然Renderable包装了Geom的类, 那么将Geom类的接口也提供同样的一份, 是十分必要的, 因为可以缩短后续访问Renderable内部Geom对象的代码长度, 而且有利于将ElementOfRenderable的上级Renderable的指针和当前的Renderable对象绑定

    Models::Import();

    // TODO Here, make sure the IA and Mesh will not be cycle refer here, for example, you want to init IA, but did not find mesh and shader
    
    // it would say, make Geometry also hold the memory of Mesh, Shader, IA, than it will make the scene easy
    // but, it is not good when you have a big scene and you want to reuse others
    m_pGLRect->setMesh(Models::m_pGLMesh);
    m_pGLRect->setShader(Models::m_pGLSL);
    m_pGLRect->addInputVertexAttribute(Models::m_IAPos);
    m_pGLRect->addInputVertexAttribute(Models::m_IAColor);

    m_pRenderablesVector->push_back(m_pGLRect);

    // Note
    // Scene will new/delete the Renderables Vector
    // Renderable will new/delete the Geometry<GLMesh, GLSLShader, GLInputVertexAttribute>
    // Geometry will not new/delete the Mesh, Shader, IA

    return true;
}

bool GLBasicScene::UnLoad()
{
    if (m_pRenderablesVector)
    {
        for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
        {
            // TODO, check what will happen, when it's been Init() or Destroy() twice or more
            // TODO, check what will happen, when it's been delete twice or more
            if (*i)
            {
                (*i)->Destroy();
                delete (*i);
                (*i) = NULL;
            }
        }

        m_pRenderablesVector->erase(m_pRenderablesVector->begin(), m_pRenderablesVector->end());
    }
    return true;
}