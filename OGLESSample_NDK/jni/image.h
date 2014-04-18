#ifndef AY_IMAGE
#define AY_IMAGE

#include <string>
#include <png.h>

#include "resource.h"
#include "type_defines.h"

class PNG
{
public:
    explicit PNG(ReadResource* pResource);
    virtual ~PNG();

    bool Load();
    bool UnLoad();

    // getter
    unsigned int getWidth();
    unsigned int getHeight();
    GLenum getFormat();
    BYTE* getData();
    bool hasAlpha();

private:
    static void callback_read(png_structp pStruct, png_bytep pData, png_size_t pSize);

private:
    ReadResource* m_pResource;
    BYTE* m_pData;
    png_uint_32 m_Width;
    png_uint_32 m_Height;
    GLenum m_Format;
    int m_BitDepth;
    int m_ColorType;
    int m_InterlaceType;
    int m_CompressionType;
    int m_FilterMethod;

};

#endif
