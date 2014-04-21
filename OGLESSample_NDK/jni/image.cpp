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
            break;
        if (!(m_pResource->Read(lHeader, sizeof(lHeader))))
            break;
        if (png_sig_cmp(lHeader, 0, 8) != 0)
            break;

        // Creates required structures.
        lPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!lPngPtr)
            break;
        lInfoPtr = png_create_info_struct(lPngPtr);
        if (!lInfoPtr)
            break;

        // Prepares reading operation by setting-up a read callback.
        png_set_read_fn(lPngPtr, m_pResource, PNG::callback_read);

        // Set-up error management. If an error occurs while reading,
        // code will come back here and jump
        if (setjmp(png_jmpbuf(lPngPtr)))
            break;

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
            // goto ERROR; // todo, this should be error for original source codes in Begin NDK Programming book
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
            break;

        // Ceates the image buffer that will be sent to OpenGL.
        size_t bigsize = lRowSize * m_Height;

        // todo, notebook, if the image size is 225*225 with rgb image, you will find the bigsize is 225*225*3 = 151875
        // if you new byte[151875], a segment fault will happen with msg: Cannot access memory at address 0x4278f4f8
        // use some even value will be fine
        if (bigsize % 2 != 0)
        {
            LOGD("png load find the image's size is %d, which is odd number", bigsize);
            bigsize++;
            LOGD("try to make it be even: %d", bigsize);
        }

        LOGD("begin to new memory in size: %d", bigsize);
        lImageBuffer = new png_byte[bigsize];
        LOGD("end to new memory in size: %d", bigsize);

        // todo here very much
        // make GLTexture2D for file works fine, which will fail when try to open the file in sdcard
        // notebook, when use both real device and emulator, adb may not answer the change, use adb kill-server to reset the adb
        // notebook, on android device, new a even memory size may be fail
        // notebook, how to debug on mac
        // todo here very much
        // - analysis the crash.debug.log deeply
        // - IA will use itself to init a temp var in find_if, that will call it's Disable and call null pointer about its Renderable
        // - 1st, not call disable when not necessory for all element of renderable
        // - 2nd, maybe remove the fix which just call set renderable for each item when it add to renderable, see the fix with crash.log
        // todo here very much
        // - find why the app not crash, but now it has no view
        // - make the texture file load from /sdcard/ be back, now it just be disable by #if 0 #endif
        // todo here
        // remove redundant logd here in PNG's Load()
        // todo notebook
        // - how to save breakpoints and use it when begin to debug
        // ~/android/android-ndk-r9b/ndk-gdb-py --start -x bp.gdb


        LOGD("begine to check the lImageBuffer: %x", lImageBuffer);
        if (!lImageBuffer)
            break;
        LOGD("end to check the lImageBuffer: %x", lImageBuffer);

        // Pointers to each row of the image buffer. Row order is
        // inverted because different coordinate systems are used by
        // OpenGL (1st pixel is at bottom left) and PNGs (top-left).
        LOGD("begin to new memory in size: %d", m_Height);
        lRowPtrs = new png_bytep[m_Height];
        LOGD("end to new memory in size: %d", m_Height);

        LOGD("begine to check the lRowPtrs: %x", lRowPtrs);
        if (!lRowPtrs)
            break;
        LOGD("end to check the lRowPtrs: %x", lRowPtrs);

        for (int32_t i = 0; i < m_Height; ++i)
        {
            lRowPtrs[m_Height - (i + 1)] = lImageBuffer + i * lRowSize;
        }

        LOGD("begine to png_read_image");
        // Reads image content.
        png_read_image(lPngPtr, lRowPtrs);
        LOGD("end to png_read_image");

        // Frees memory and resources.
        m_pResource->Close();
        png_destroy_read_struct(&lPngPtr, &lInfoPtr, NULL);
        delete[] lRowPtrs;
        m_pData = lImageBuffer;

        LOGD("load png success, will return true");
        LOGD("the image data is at %x", m_pData);
        return true;
    }
    while(0);

    LOGE("Error while reading PNG file: %s", m_pResource->getPath().c_str());
    m_pResource->Close();
    delete[] lRowPtrs;
    delete[] lImageBuffer;
    if (lPngPtr != NULL)
    {
        png_infop* lInfoPtrP = lInfoPtr != NULL ? &lInfoPtr : NULL;
        png_destroy_read_struct(&lPngPtr, lInfoPtrP, NULL);
    }
    m_pData = NULL;
    return false;

    // todo, notebook, write down how to remove goto, and use while to replace it
    // todo, load and unload should happen in some if condition
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
    m_Format = GL_RGBA;
    m_BitPerChannel = 0;
    m_BytePerPixel = 0;
    m_BytePerRow = 0;
    m_ColorType = 0;
    m_InterlaceType = 0;
    m_CompressionType = 0;
    m_FilterMethod = 0;

    if (m_pResource)
    {
        delete m_pResource;
        m_pResource = NULL;
    }
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
    png_get_IHDR(png, info, &m_Width, &m_Height, &m_BitPerChannel, &m_ColorType, &m_InterlaceType, &m_CompressionType, &m_FilterMethod);

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

RawImage::RawImage(BYTE* pData, unsigned int width, unsigned int height, GLenum format_ogl, GLenum type_ogl, std::string name) :
    m_Width(width),
    m_Height(height),
    m_Format(format_ogl),
    m_Type(type_ogl),
    m_Name(name),
    m_bHasAlpha(false),
    m_BytePerRow(0)
{
    // copy the image data own
    size_t channel_per_pixel, bit_per_channel, bit_per_pixel;

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
    m_BytePerRow = width * (bit_per_pixel / 8);

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
    if (m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }
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





