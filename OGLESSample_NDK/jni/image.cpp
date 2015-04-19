#include "type_defines.h"
#include "image.h"
#include "log.h"

PNG::PNG(ReadResource* pResource) :
    Image(),
    m_pData(NULL),
    m_Width(0),
    m_Height(0),
    m_Format(GL_RGBA),
    m_BitPerChannel(0),
    m_BytePerPixel(0),
    m_BytePerRow(0),
    m_ColorType(0),
    m_InterlaceType(0),
    m_CompressionType(0),
    m_FilterMethod(0),
    m_pResource(pResource)
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
    png_int_32 lBitDepth;
    bool lTransparency;

    do
    {
        // Opens and checks image signature (first 8 bytes).
        if (m_pResource->Open() != true)
        {
            LOGE("PNG::Load() can't call m_pResource's Open()");
            break;
        }
        if (!(m_pResource->Read(lHeader, sizeof(lHeader))))
        {
            LOGE("PNG::Load() can't call m_pResource's Read() for 8 bytes");
            break;
        }
        if (png_sig_cmp(lHeader, 0, 8) != 0)
        {
            LOGE("PNG::Load() can't compare 8 bytes");
            break;
        }

        // Creates required structures.
        lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!lPngPtr)
        {
            LOGE("PNG::Load() can't successfully call png_create_read_struct");
            break;
        }
        lInfoPtr = png_create_info_struct(lPngPtr);
        if (!lInfoPtr)
        {
            LOGE("PNG::Load() can't successfully call png_create_info_struct");
            break;
        }

        // Prepares reading operation by setting-up a read callback.
        png_set_read_fn(lPngPtr, m_pResource, PNG::callback_read);

        // Set-up error management. If an error occurs while reading,
        // code will come back here and jump
        if (setjmp(png_jmpbuf(lPngPtr)))
        {
            LOGE("PNG::Load() can't successfully call setjmp(png_jmpbuff))");
            break;
        }

        // Ignores first 8 bytes already read and processes header.
        png_set_sig_bytes(lPngPtr, 8);
        png_read_info(lPngPtr, lInfoPtr);

        // Retrieves PNG info and updates PNG structure accordingly.
        png_get_IHDR(lPngPtr, lInfoPtr, &m_Width, &m_Height, &lBitDepth, &m_ColorType, &m_InterlaceType, &m_CompressionType, &m_FilterMethod);

        // Creates a full alpha channel if transparency is encoded as
        // an array of palette entries or a single transparent color.
        lTransparency = false;
        if (png_get_valid(lPngPtr, lInfoPtr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(lPngPtr);
            lTransparency = true;
        }

        // Expands PNG with less than 8Bits per channel to 8Bits.
        if (lBitDepth < 8)
        {
            png_set_packing (lPngPtr);
            m_BitPerChannel = 8;
        }
        // Shrinks PNG with 16Bits per color channel down to 8Bits.
        else if (lBitDepth == 16)
        {
            png_set_strip_16(lPngPtr);
            m_BitPerChannel = 8;
        }
        else
        {
            m_BitPerChannel = lBitDepth;
        }

        // Indicates that image needs conversion to RGBA if needed.
        switch (m_ColorType)
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
            m_Format = lTransparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_GA:
            png_set_expand_gray_1_2_4_to_8(lPngPtr);
            m_Format = GL_LUMINANCE_ALPHA;
            break;
            // todo, what about the format GL_ALPHA
        }

        switch (m_Format)
        {
        case GL_RGBA:
            m_BytePerPixel = 4 * m_BitPerChannel / 8;
            break;
        case GL_RGB:
            m_BytePerPixel = 3 * m_BitPerChannel / 8;
            break;
        case GL_LUMINANCE_ALPHA:
            m_BytePerPixel = 2 * m_BitPerChannel / 8;
            break;
        case GL_LUMINANCE:
            m_BytePerPixel = 1 * m_BitPerChannel / 8;
            break;
            // todo, what about the format GL_ALPHA
        }

        // Validates all transformations.
        png_read_update_info(lPngPtr, lInfoPtr);

        // Get row size in bytes.
        lRowSize = png_get_rowbytes(lPngPtr, lInfoPtr);
        m_BytePerRow = lRowSize;
        if (lRowSize <= 0)
        {
            LOGE("PNG::Load() can't successfully call png_get_rowbytes");
            break;
        }

        // Ceates the image buffer that will be sent to OpenGL.
        size_t bigsize = lRowSize * m_Height;

        // use some even value will be fine
        if (bigsize % 2 != 0)
        {
            LOGD("png load find the image's size is %d, which is odd number", bigsize);
            bigsize++;
            LOGD("try to make it be even: %d", bigsize);
        }

        lImageBuffer = new png_byte[bigsize];

        if (!lImageBuffer)
        {
            LOGE("PNG::Load() can't successfully new memory for lImageBuffer");
            break;
        }

        // Pointers to each row of the image buffer. Row order is
        // inverted because different coordinate systems are used by
        // OpenGL (1st pixel is at bottom left) and PNGs (top-left).
        lRowPtrs = new png_bytep[m_Height];

        if (!lRowPtrs)
        {
            LOGE("PNG::Load() can't successfully new memory for lRowPtrs");
            break;
        }

        for (int32_t i = 0; i < m_Height; ++i)
        {
            lRowPtrs[m_Height - (i + 1)] = lImageBuffer + i * lRowSize;
        }

        // Reads image content.
        png_read_image(lPngPtr, lRowPtrs);

        // Frees memory and resources.
        m_pResource->Close();
        png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
        SAFE_DELETE_ARRAY(lRowPtrs);
        m_pData = lImageBuffer;

        LOGD("load png %s success, will return true", m_pResource->getPath().c_str());
        return true;
    }
    while(0);

    LOGE("Error while reading PNG file: %s", m_pResource->getPath().c_str());
    m_pResource->Close();
    SAFE_DELETE_ARRAY(lRowPtrs);
    SAFE_DELETE_ARRAY(lImageBuffer);
    if (lPngPtr != NULL)
    {
        png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr : NULL;
        png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
    }
    m_pData = NULL;
    return false;
}

bool PNG::UnLoad()
{
    SAFE_DELETE_ARRAY(m_pData);
    m_Width = 0;
    m_Height = 0;
    m_Format = GL_RGBA;
    m_BitPerChannel = 0;
    m_BytePerPixel = 0;
    m_BytePerRow = 0;
    m_ColorType = 0;
    m_InterlaceType = 0;
    m_CompressionType = 0;
    m_FilterMethod = 0;

    SAFE_DELETE(m_pResource);
}

unsigned int PNG::getWidth()
{
    return m_Width;
}

unsigned int PNG::getHeight()
{
    return m_Height;
}

bool PNG::hasAlpha()
{
    if (m_ColorType == PNG_COLOR_TYPE_RGBA)
    {
        return true;
    }
    return false;
}

unsigned int PNG::getBytePerRow()
{
    size_t row_size = m_BytePerPixel * m_Width;
    if (row_size != m_BytePerRow)
    {
        LOGD("Row Size get from libpng and calculate by byte per pixel * width is Different");
    }
    return m_BytePerRow;
}

GLint PNG::getInternalFormatInOGL()
{
    // todo, in PNG load, didn't distinguish the internal format of ogl and format of pixel data
    return m_Format;
}

GLenum PNG::getFormatInOGL()
{
    return m_Format;
}

GLenum PNG::getTypeInOGL()
{
    if ( m_BitPerChannel == 8)
    {
        return GL_UNSIGNED_BYTE;
    }
    else if (m_BitPerChannel == 16)
    {
        LOGD("the loading png image %s has %d bit width per channel, which may not be supported as type in glTexImage2D of OGLES", getName().c_str(), 16);
    }
    else if (m_BitPerChannel == 32)
    {
        LOGD("the loading png image %s has %d bit width per channel, which may not be supported as type in glTexImage2D of OGLES", getName().c_str(), 32);
    }

    if (m_BytePerPixel == 2)
    {
        if (m_Format == GL_RGBA)
        {
            // todo, find some way to support GL_UNSIGNED_SHORT_4_4_4_4
            // todo, find some way to support GL_UNSIGNED_SHORT_5_5_5_1
            LOGD("the loading png image %s has %d bit per pixel, and it's GL_RGBA format", getName().c_str(), 16);
            LOGD("but we have no way [in libpng] to determine whether the type in glTexImage2D of OGLES is suit for GL_UNSIGNED_SHORT_4_4_4_4 or GL_UNSIGNED_SHORT_5_5_5_1 or something else");
        }

        if (m_Format == GL_RGB)
        {
            // todo, find some way to support GL_UNSIGNED_SHORT_5_6_5
            LOGD("the loading png image %s has %d bit per pixel, and it's GL_RGB format", getName().c_str(), 16);
            LOGD("but we have no way [in libpng] to determine whether the type in glTexImage2D of OGLES is suit for GL_UNSIGNED_SHORT_5_6_5 or something else");
        }
    } 

    return GL_UNSIGNED_BYTE;
}

unsigned char* PNG::getData()
{
    return m_pData;
}

std::string PNG::getName() const
{
    return m_pResource->getPath();
}

RawImage::RawImage(BYTE* pData, unsigned int width, unsigned int height, unsigned int bit_per_pixel, GLint internal_format_gl, GLenum format_ogl, GLenum type_ogl, std::string name) :
    m_pData(nullptr),
    m_Width(width),
    m_Height(height),
    m_InternalFormat(internal_format_gl),
    m_Format(format_ogl),
    m_Type(type_ogl),
    m_Name(name),
    m_bHasAlpha(false),
    m_BytePerRow(0)
{
#if !defined(USE_GLES3)
    size_t channel_per_pixel, bit_per_channel;

    switch (format_ogl)
    {
    case GL_RGBA:
        channel_per_pixel = 4;
        m_bHasAlpha = true;
        break;
    case GL_RGB:
        channel_per_pixel = 3;
        m_bHasAlpha = false;
        break;
    case GL_LUMINANCE_ALPHA:
        channel_per_pixel = 2;
        m_bHasAlpha = true;
        break;
    case GL_LUMINANCE:
        channel_per_pixel = 1;
        m_bHasAlpha = false;
        break;
    case GL_ALPHA:
        channel_per_pixel = 1;
        m_bHasAlpha = true;
        break;
    }

    if (!bit_per_pixel)
    {
        switch (type_ogl)
        {
        case GL_UNSIGNED_BYTE:
            bit_per_channel = 8;
            bit_per_pixel = channel_per_pixel * bit_per_channel;
            break;
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_5_6_5:
            bit_per_pixel = 16;
            break;
        }
    }
#else
    switch (m_Format)
    {
    case GL_RGBA:
    case GL_RGBA_INTEGER:
    case GL_LUMINANCE_ALPHA:
    case GL_ALPHA:
        m_bHasAlpha = true;
        break;
    default:
        m_bHasAlpha = false;
        break;
    }
#endif

    // set byte per row
    m_BytePerRow = width * (bit_per_pixel / 8);

    // copy original raw data to RawImage's own memory
    size_t size = width * height * (bit_per_pixel / 8);
    m_pData = new BYTE[size];
    memcpy(m_pData, pData, size);

}

RawImage::~RawImage()
{
    UnLoad();
}

bool RawImage::Load()
{
    return true;
}

bool RawImage::UnLoad()
{
    SAFE_DELETE_ARRAY(m_pData);
    m_Width = 0;
    m_Height = 0;
    m_bHasAlpha = false;
    m_BytePerRow = 0;
    return true;
}

unsigned int RawImage::getWidth()
{
    return m_Width;
}

unsigned int RawImage::getHeight()
{
    return m_Height;
}

bool RawImage::hasAlpha()
{
    return m_bHasAlpha;
}

unsigned int RawImage::getBytePerRow()
{
    return m_BytePerRow;
}

GLint RawImage::getInternalFormatInOGL()
{
    return m_InternalFormat;
}

GLenum RawImage::getFormatInOGL()
{
    return m_Format;
}

GLenum RawImage::getTypeInOGL()
{
    return m_Type;
}

BYTE* RawImage::getData()
{
    return m_pData;
}

std::string RawImage::getName() const
{
    return m_Name;
}





