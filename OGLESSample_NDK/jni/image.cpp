#include "image.h"
#include "log.h"

PNG::PNG(ReadResource* pResource) :
    m_pResource(pResource),
    m_pData(NULL),
    m_Width(0),
    m_Height(0),
    m_Format(GL_RGBA),
    m_BitDepth(0),
    m_ColorType(0),
    m_InterlaceType(0),
    m_CompressionType(0),
    m_FilterMethod(0)
{
}

PNG::~PNG()
{
    UnLoad();
}

void PNG::callback_read(png_structp pStruct, png_bytep pData, png_size_t pSize)
{
    ReadResource* lResource = ((ReadResource*) png_get_io_ptr(pStruct));
    if (lResource->Read(pData, pSize) <= 0)
    {
        lResource->Close();
    }
}

bool PNG::Load()
{
    png_byte lHeader[8];
    png_structp lPngPtr = NULL;
    png_infop lInfoPtr = NULL;
    png_byte* lImageBuffer = NULL;
    png_bytep* lRowPtrs = NULL;
    png_int_32 lRowSize;
    bool lTransparency;

    // Opens and checks image signature (first 8 bytes).
    if (m_pResource->Open() != true)
        goto ERROR;
    if (m_pResource->Read(lHeader, sizeof(lHeader)) != true)
        goto ERROR;
    if (png_sig_cmp(lHeader, 0, 8) != 0)
        goto ERROR;

    // Creates required structures.
    lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!lPngPtr)
        goto ERROR;
    lInfoPtr = png_create_info_struct(lPngPtr);
    if (!lInfoPtr)
        goto ERROR;

    // Prepares reading operation by setting-up a read callback.
    png_set_read_fn(lPngPtr, m_pResource, PNG::callback_read);

    // Set-up error management. If an error occurs while reading,
    // code will come back here and jump
    if (setjmp(png_jmpbuf(lPngPtr)))
        goto ERROR;

    // Ignores first 8 bytes already read and processes header.
    png_set_sig_bytes(lPngPtr, 8);
    png_read_info(lPngPtr, lInfoPtr);

    // Retrieves PNG info and updates PNG structure accordingly.
    png_int_32 lDepth, lColorType;
    png_uint_32 lWidth, lHeight;
    png_get_IHDR(lPngPtr, lInfoPtr, &lWidth, &lHeight, &lDepth, &lColorType, NULL, NULL, NULL);
    m_Width = lWidth;
    m_Height = lHeight;

    // Creates a full alpha channel if transparency is encoded as
    // an array of palette entries or a single transparent color.
    lTransparency = false;
    if (png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(lPngPtr);
        lTransparency = true;
        // goto ERROR; // todo, this should be error for original source codes in Begin NDK Programming book
    }

    // Expands PNG with less than 8Bits per channel to 8Bits.
    if (lDepth < 8)
    {
        png_set_packing (lPngPtr);
    }
    // Shrinks PNG with 16Bits per color channel down to 8Bits.
    else if (lDepth == 16)
    {
        png_set_strip_16(lPngPtr);
    }

    // Indicates that image needs conversion to RGBA if needed.
    switch (lColorType)
    {
    case PNG_COLOR_TYPE_PALETTE:
        png_set_palette_to_rgb(lPngPtr);
        m_Format = lTransparency ? GL_RGBA : GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGB:
        m_Format = lTransparency ? GL_RGBA : GL_RGB;
        break;
    case PNG_COLOR_TYPE_RGBA:
        m_Format = GL_RGBA;
        break;
    case PNG_COLOR_TYPE_GRAY:
        png_set_expand_gray_1_2_4_to_8(lPngPtr);
        m_Format = lTransparency ? GL_LUMINANCE_ALPHA:GL_LUMINANCE;
        break;
    case PNG_COLOR_TYPE_GA:
        png_set_expand_gray_1_2_4_to_8(lPngPtr);
        m_Format = GL_LUMINANCE_ALPHA;
        break;
    }
    // Validates all transformations.
    png_read_update_info(lPngPtr, lInfoPtr);

    // Get row size in bytes.
    lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
    if (lRowSize <= 0)
        goto ERROR;
    // Ceates the image buffer that will be sent to OpenGL.
    lImageBuffer = new png_byte[lRowSize * lHeight];
    if (!lImageBuffer)
        goto ERROR;
    // Pointers to each row of the image buffer. Row order is
    // inverted because different coordinate systems are used by
    // OpenGL (1st pixel is at bottom left) and PNGs (top-left).
    lRowPtrs = new png_bytep[lHeight];
    if (!lRowPtrs)
        goto ERROR;
    for (int32_t i = 0; i < lHeight; ++i)
    {
        lRowPtrs[lHeight - (i + 1)] = lImageBuffer + i * lRowSize;
    }
    // Reads image content.
    png_read_image(lPngPtr, lRowPtrs);

    // Frees memory and resources.
    m_pResource->Close();
    png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
    delete[] lRowPtrs;
    return lImageBuffer;

ERROR:
    LOGE("Error while reading PNG file");
    m_pResource->Close();
    delete[] lRowPtrs;
    delete[] lImageBuffer;
    if (lPngPtr != NULL)
    {
        png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr : NULL;
        png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
    }
    return NULL;
}

bool PNG::UnLoad()
{
    if (m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
    m_Width = 0;
    m_Height = 0;
    m_BitDepth = 0;
    m_ColorType = 0;
    m_InterlaceType = 0;
    m_CompressionType = 0;
    m_FilterMethod = 0;
}

#if 0
void LoadPNGFile(std::string const file_name)
{
    FILE* fp = fopen(file_name.c_str(), "rb");
    if (fp == NULL)
    {
        LOGE("%s is not found.", file_name.c_str());
        fclose(fp);
        return;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        LOGE("png_create_read_struct error.");
        fclose(fp);
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (info == NULL) 
    {
        LOGE("png_create_info_struct error.");
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png))) 
    {
        LOGE("png_jmpbuf error.");
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png, fp);

    unsigned int sig_bytes = 0;
    png_set_sig_bytes(png, sig_bytes);

    png_read_png(png, info, (PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND), NULL);
    png_get_IHDR(png, info, &m_Width, &m_Height, &m_BitDepth, &m_ColorType, &m_InterlaceType, &m_CompressionType, &m_FilterMethod);

    unsigned int row_bytes = png_get_rowbytes(png, info);
    if (m_pData != NULL)
    {
        delete[] m_pData;
    }
    m_pData = new unsigned char[row_bytes * m_Height];

    png_bytepp rows = png_get_rows(png, info);
    for (int i = 0; i < m_Height; ++i)
    {
        memcpy(m_pData + (row_bytes * i), rows[i], row_bytes);
    }

    png_destroy_read_struct(&png, &info, NULL);
}
#endif

unsigned int PNG::getWidth()
{
    return m_Width;
}

unsigned int PNG::getHeight()
{
    return m_Height;
}

GLenum PNG::getFormat()
{
    return m_Format;
}

unsigned char* PNG::getData()
{
    return m_pData;
}

bool PNG::hasAlpha()
{
    if (m_ColorType == PNG_COLOR_TYPE_RGBA)
    {
        return true;
    }
    return false;
}

// todo here, add other getter for bit depth, interlace type, compression type, filtermethod, etc
