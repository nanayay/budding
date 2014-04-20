#ifndef AY_IMAGE
#define AY_IMAGE

#include <string>
#include <png.h>

#include "resource.h"
#include "type_defines.h"

class Image
{
public:
    explicit Image() {};
    virtual ~Image() {};

    virtual bool Load() = 0;
    virtual bool UnLoad() = 0;

    virtual unsigned int getWidth() = 0;
    virtual unsigned int getHeight() = 0;
    virtual bool hasAlpha() = 0;
    virtual unsigned int getBytePerRow() = 0;
    virtual GLenum getFormatInOGL() = 0;
    virtual GLenum getTypeInOGL() = 0;
    virtual BYTE* getData() = 0;
    virtual std::string getName() const = 0;

};

class PNG
    : public Image
{
public:
    explicit PNG(ReadResource* pResource);
    virtual ~PNG();

    virtual bool Load();
    virtual bool UnLoad();

    virtual unsigned int getWidth();
    virtual unsigned int getHeight();
    virtual bool hasAlpha();
    virtual unsigned int getBytePerRow();
    virtual GLenum getFormatInOGL();
    virtual GLenum getTypeInOGL();
    virtual BYTE* getData();
    virtual std::string getName() const;


private:
    static void callback_read(png_structp pStruct, png_bytep pData, png_size_t pSize);

private:
    BYTE* m_pData;
    png_uint_32 m_Width;
    png_uint_32 m_Height;
    GLenum m_Format;
    unsigned int m_BytePerRow;

    png_int_32 m_BitPerChannel;
    unsigned int m_BytePerPixel;
    png_int_32 m_ColorType;
    png_int_32 m_InterlaceType;
    png_int_32 m_CompressionType;
    png_int_32 m_FilterMethod;

    ReadResource* m_pResource;

};

class RawImage
    : public Image
{
public:
    explicit RawImage(BYTE* pData, unsigned int width, unsigned int height, GLenum format_ogl, GLenum type_ogl, std::string name);
    virtual ~RawImage();

    virtual bool Load();
    virtual bool UnLoad();

    virtual unsigned int getWidth();
    virtual unsigned int getHeight();
    virtual bool hasAlpha();
    virtual unsigned int getBytePerRow();
    virtual GLenum getFormatInOGL();
    virtual GLenum getTypeInOGL();
    virtual BYTE* getData();
    virtual std::string getName() const;

private:
    BYTE* m_pData;
    unsigned int m_Width;
    unsigned int m_Height;
    GLenum m_Format;
    GLenum m_Type;
    std::string m_Name;

    bool m_bHasAlpha;
    unsigned int m_BytePerRow;

};

#endif
