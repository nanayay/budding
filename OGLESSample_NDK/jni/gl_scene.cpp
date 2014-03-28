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
    #if 0
        m_pGLMesh = new GLMesh(true); // true will use client buffer, false [default] will use gpu buffer
    #else
        m_pGLMesh = new GLMesh();
    #endif
        m_pGLSL = new GLSLShader(&vss, &fss);
        m_IAPos = new GLInputVertexAttribute(ia_pos);
        m_IAColor = new GLInputVertexAttribute(ia_color);

        // TODO Here, make the GLSLShader can also accept the char* as input
        //GLSLShader* m_pGLSL = new GLSLShader(vs, fs);
        // TODO Here, the GLSL's pointer will be undefined if out-of-scope

        m_pGLMesh->setVertexDataPointer(vertes);
        m_pGLMesh->setVertexDataSize(sizeof(vertes));
        m_pGLMesh->setIndexDataPointer(indices);
        m_pGLMesh->setIndexDataSize(sizeof(indices));
        m_pGLMesh->setNumOfIndices(num_of_index);
        m_pGLMesh->setIndexDataOffset(0);

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

        // Todo Notebook
        // Note, can not call these Create() here, if you call it, when you call GLRenderable::Draw()'s glDrawElements will show crash
        // This crash is very hard to debug, since it is not part of glGetError, nor part of gdb catch-able crash, only test-try-log can help this, you can see the debug.create.later.log and debug.create.first.log to compare
        // The root of this crash is when the Create() is calling glFoo, the EGL is not init yet, you must wait the EGL be initialized done, then call these Create()

        // TODO here, make GLScene's Import() can call Create() after EGL init finished
        //m_pGLMesh->Create();
        //m_pGLSL ->Create();
        //m_IAPos->Create();
        //m_IAColor->Create();

        return true;
    }

    bool DisImport()
    {
        // TODO, find some place to Call DisImport to ignore the memory leak about ElementOfRenderable
        m_pGLMesh->Dispose();
        m_pGLSL->Dispose();
        m_IAPos->Dispose();
        m_IAColor->Dispose();

        delete m_pGLMesh;
        delete m_pGLSL;
        delete m_IAPos;
        delete m_IAColor;

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

    Models::Import();

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

    Models::DisImport();

    return true;
}