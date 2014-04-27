#include <algorithm>
#include "gl_scene.h"
#include "image.h"
#include "android_asset.h"

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
        -0.5, 0.5, 0.0, // Pos 0 
        1.0, 0.0, 0.0, 1.0, // Color 0
        0.0, 1.0, // Tex Coordinate 0

        0.5, 0.5, 0.0, // Pos 1
        0.0, 1.0, 0.0, 1.0, // Color 1
        1.0, 1.0, // Tex Coordinate 1

        -0.5, -0.5, 0.0, // Pos 2
        0.0, 0.0, 1.0, 1.0, // Color 2
        0.0, 0.0, // Tex Coordinate 2

        0.5, -0.5, 0.0, // Pos 3
        1.0, 1.0, 1.0, 1.0, // Color 3
        1.0, 0.0 // Tex Coordinate 3
    };

    unsigned int stride = sizeof(vertes)/num_of_vertex;
    unsigned int num_of_element_in_pos_for_one_vertex = 3;
    unsigned int num_of_element_in_color_for_one_vertex = 4;
    unsigned int num_of_element_in_texCoord_for_one_vertex = 2;

    size_t offset_of_pos = 0;
    size_t offset_of_color = offset_of_pos + num_of_element_in_pos_for_one_vertex * sizeof(float);
    size_t offset_of_texCoord = offset_of_color + num_of_element_in_color_for_one_vertex * sizeof(float);

    unsigned short indices [] =
    {
        0, 2, 1,
        2, 3, 1
    };
    unsigned int num_of_index = sizeof(indices)/sizeof(indices[0]);

    std::string ia_pos("a_vPosition");
    std::string ia_color("a_vColor");
    std::string ia_texCoord("a_vTexCoord");

    const char* vs = 
        "#version 100\n"
        "attribute vec4 a_vPosition;\n"
        "attribute vec4 a_vColor;\n"
        "attribute vec2 a_vTexCoord;\n"
        "varying vec4 v_vColor;\n"
        "varying vec2 v_vTexCoord;\n"
        "void main()\n"
        "{\n"
            "gl_Position = a_vPosition;\n"
            "v_vColor = a_vColor;\n"
            "v_vTexCoord = a_vTexCoord;\n"
        "}\n";

    const char* fs =
        "#version 100\n"
        //"precision lowp float;\n"
        //"precision mediump float;\n"
        "precision highp float;\n"
        "varying vec4 v_vColor;\n"
        "varying vec2 v_vTexCoord;\n"
        "uniform sampler2D u_sampleTexture2D_0;\n"
        "uniform sampler2D u_sampleTexture2D_1;\n"
        "uniform sampler2D u_sampleTexture2D_2;\n"
        "void main()\n"
        "{\n"
            "gl_FragColor = 0.1 * v_vColor + 0.2 * texture2D(u_sampleTexture2D_0, v_vTexCoord) + 0.4 * texture2D(u_sampleTexture2D_1, v_vTexCoord) + 0.2 * texture2D(u_sampleTexture2D_2, v_vTexCoord);\n"
            // todo here, notebook, if you not use v_vColor, the compiler will optimized color, then in glGetAttribLocation to get that attribute, it will be failed
        "}\n";

    std::string vss(vs);
    std::string fss(fs);

    GLMesh* m_pGLMesh = NULL;
    GLSLShader* m_pGLSL = NULL;
    GLInputVertexAttribute* m_pIAPos = NULL;
    GLInputVertexAttribute* m_pIAColor = NULL;
    GLInputVertexAttribute* m_pIATexCoord = NULL;
    GLTexture2D* m_pTex2DChess = NULL;
    GLTexture2D* m_pTex2DSDCard = NULL;
    GLTexture2D* m_pTex2DAsset = NULL;
    GLSampler* m_pSampler = NULL;

    // pixel in hard-code
    std::string tex_chess_name = std::string("chess_2x2");
    std::string tex_chess_uniform_name = std::string("u_sampleTexture2D_0");
    unsigned int tex_chess_unit_id = 0;

    int tex_chess_width = 2;
    int tex_chess_height = 2;
    GLenum tex_chess_pixels_format = GL_RGBA;
    GLenum tex_chess_pixel_type = GL_UNSIGNED_BYTE;
    GLubyte tex_chess_pixels[4*4] =
    {
        255, 0, 0, 255,
        0, 255, 0, 255,
        0, 0, 255, 255,
        255, 255, 0, 255,
    };

#if 1 
    // load png file in sdcard
    std::string tex_sdcard_filepath = std::string("/sdcard/ayan/android_icon_sdcard.png");
    std::string tex_sdcard_uniform_name = std::string("u_sampleTexture2D_1");
    unsigned int tex_sdcard_unit_id = 1;

    // todo here, why un-comment these two below will make compile fail
    //::LOGD("Debug info for texture in sdcard");
    //::LOGD("Image %s : alpha: %d width %dpx height %dpx", tex_sdcard_filepath.c_str(), tex_sdcard_png.has_alpha(), tex_sdcard_png.get_width(), tex_sdcard_png.get_height());

    // todo here, if the sdcard.png not exist in device or emulator, it will crash
    // todo here, notebook, how to make emulator be write-able, and restart will missing the data in sdcard folder
    // todo here, notebook, adb push sdcard.png /sdcard/ayan will rename sdcard.png locally to ayan, which not means to /sdcard/ayan/sdcard.png, but just sdcard/ayan, since ayan is only a file
    // correct way is only adb push sdcard.png /sdcard/ayan/sdcard.png
#endif

    // load png file in apk's asset
    std::string tex_asset_filepath = std::string("kitkat_with_alpha.png");
    std::string tex_asset_uniform_name = std::string("u_sampleTexture2D_2");
    unsigned int tex_asset_unit_id = 2;


    bool Import()
    {
        // for the raw image data in hard code
        RawImage* raw_chess_texture = new RawImage(Models::tex_chess_pixels, Models::tex_chess_width, Models::tex_chess_height, Models::tex_chess_pixels_format, Models::tex_chess_pixel_type, Models::tex_chess_name);

        // for the png file in sdcard
        ReadFile* tex_sdcard_png_readfile = new ReadFile(Models::tex_sdcard_filepath);
        PNG* tex_sdcard_png = new PNG(tex_sdcard_png_readfile);

        // for the android asset png file
        AndroidAsset* tex_asset_png_readasset = new AndroidAsset(Models::tex_asset_filepath);
        PNG* tex_asset_png = new PNG(tex_asset_png_readasset);

    #if 0
        m_pGLMesh = new GLMesh(true); // true will use client buffer, false [default] will use gpu buffer
    #else
        m_pGLMesh = new GLMesh();
    #endif
        m_pGLSL = new GLSLShader(&vss, &fss);
        m_pIAPos = new GLInputVertexAttribute(ia_pos);
        m_pIAColor = new GLInputVertexAttribute(ia_color);
        m_pIATexCoord = new GLInputVertexAttribute(ia_texCoord);
        m_pTex2DChess = new GLTexture2D(std::string("Texture0"), Models::tex_chess_uniform_name, Models::tex_chess_unit_id, raw_chess_texture);
    #if 1
        m_pTex2DSDCard = new GLTexture2D(std::string("Texture1"), Models::tex_sdcard_uniform_name, Models::tex_sdcard_unit_id, tex_sdcard_png);
    #endif
        m_pTex2DAsset = new GLTexture2D(std::string("Texture2"), Models::tex_asset_uniform_name, Models::tex_asset_unit_id, tex_asset_png);
        m_pSampler = new GLSampler();

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
        m_pIAPos->setNormalized(false);
        m_pIAPos->setStride(stride);
        m_pIAPos->setType(GL_FLOAT);
        m_pIAPos->setElementNum(num_of_element_in_pos_for_one_vertex);
        m_pIAPos->setOffset(offset_of_pos);
        
        m_pIAColor->setNormalized(false);
        m_pIAColor->setStride(stride);
        m_pIAColor->setType(GL_FLOAT);
        m_pIAColor->setElementNum(num_of_element_in_color_for_one_vertex);
        m_pIAColor->setOffset(offset_of_color);

        m_pIATexCoord->setNormalized(false);
        m_pIATexCoord->setStride(stride);
        m_pIATexCoord->setType(GL_FLOAT);
        m_pIATexCoord->setElementNum(num_of_element_in_texCoord_for_one_vertex);
        m_pIATexCoord->setOffset(offset_of_texCoord);

        m_pSampler->addSamplerParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        m_pSampler->addSamplerParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        m_pSampler->addSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        m_pSampler->addSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        m_pTex2DChess->setTextureSampler(m_pSampler);

    #if 1 
        m_pTex2DSDCard->setTextureSampler(m_pSampler);
    #endif

        m_pTex2DAsset->setTextureSampler(m_pSampler);

        // Todo Notebook
        // Note, can not call these Create() here, if you call it, when you call GLRenderable::Draw()'s glDrawElements will show crash
        // This crash is very hard to debug, since it is not part of glGetError, nor part of gdb catch-able crash, only test-try-log can help this, you can see the debug.create.later.log and debug.create.first.log to compare
        // The root of this crash is when the Create() is calling glFoo, the EGL is not init yet, you must wait the EGL be initialized done, then call these Create()

        m_pGLMesh->Create();
        m_pGLSL ->Create();
        m_pIAPos->Create();
        m_pIAColor->Create();
        m_pIATexCoord->Create();
        m_pTex2DChess->Create();
    #if 1 
        m_pTex2DSDCard->Create();
    #endif
        m_pTex2DAsset->Create();
        m_pSampler->Create();

        return true;
    }

    bool DisImport()
    {
        // todo here, very much
        // - check each dispose(), make sure after it delete something then assign NULL or invalid value to the delete one, like texture handle
        // - add some safe_delete here for delete call
        // - when code safe_delete, use some if (p) { delete p; p = NULL; }
        // - also add the if check in dispose()

        m_pGLMesh->Dispose();
        m_pGLSL->Dispose();
        m_pIAPos->Dispose();
        m_pIAColor->Dispose();
        m_pIATexCoord->Dispose();
        m_pSampler->Dispose();
        m_pTex2DChess->Dispose();
    #if 1 
        m_pTex2DSDCard->Dispose();
    #endif
        m_pTex2DAsset->Dispose();

        SAFE_DELETE(m_pGLMesh);
        SAFE_DELETE(m_pGLSL);
        SAFE_DELETE(m_pIAPos);
        SAFE_DELETE(m_pIAColor);
        SAFE_DELETE(m_pIATexCoord);
        SAFE_DELETE(m_pSampler);
        SAFE_DELETE(m_pTex2DChess);
    #if 0
        SAFE_DELETE(m_pTex2DSDCard);
    #endif
        SAFE_DELETE(m_pTex2DAsset);

        return true;
    }


};

bool GLBasicScene::Load()
{
    // Clear the Color and Depth buffer
    GLClearRenderable* m_pGlClear = new GLClearRenderable();

    m_pGlClear->setClearColorRGB(1.0, 1.0, 0.0, 1.0);

    m_pRenderablesVector->push_back(m_pGlClear);

    // Draw a Rectangle
    GLRenderable* m_pGLRect = new GLRenderable("Rectangle");

    Models::Import();

    // it would say, make Geometry also hold the memory of Mesh, Shader, IA, than it will make the scene easy
    // but, it is not good when you have a big scene and you want to reuse others
    m_pGLRect->setMesh(Models::m_pGLMesh);
    m_pGLRect->setShader(Models::m_pGLSL);
    m_pGLRect->addInputVertexAttribute(Models::m_pIAPos);
    m_pGLRect->addInputVertexAttribute(Models::m_pIAColor);
    m_pGLRect->addInputVertexAttribute(Models::m_pIATexCoord);
    m_pGLRect->addTexture(Models::m_pTex2DChess);
#if 1
    m_pGLRect->addTexture(Models::m_pTex2DSDCard);
#endif
    m_pGLRect->addTexture(Models::m_pTex2DAsset);

    // todo here verymuch, seem the gdb stops at addTexture finished, so next is to step over 291->294
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
            if (*i)
            {
                (*i)->Destroy();
                SAFE_DELETE((*i));
            }
        }

        m_pRenderablesVector->erase(m_pRenderablesVector->begin(), m_pRenderablesVector->end());
    }

    Models::DisImport();

    return true;
}

GLBasicScene::GLBasicScene()
{
}

GLBasicScene::~GLBasicScene()
{
    if (m_pRenderablesVector)
    {
        for (std::vector<Renderable*>::iterator i = m_pRenderablesVector->begin(); i != m_pRenderablesVector->end(); ++i)
        {
            if (*i)
            {
                (*i)->Destroy();

                SAFE_DELETE((*i));
            }
        }

        m_pRenderablesVector->erase(m_pRenderablesVector->begin(), m_pRenderablesVector->end());
    }
}