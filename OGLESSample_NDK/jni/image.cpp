#include "image.h"
#include "log.h"

PNG::PNG(const std::string& file_name) :
    file_name_(file_name), data_(NULL),
    width_(0), height_(0),
    bit_depth_(0), color_type_(0), interlace_type_(0),
    compression_type_(0), filter_method_(0)
{
    FILE* fp = fopen(file_name_.c_str(), "rb");
    if (fp == NULL)
    {
        LOGE("%s is not found.", file_name_.c_str());
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
    png_get_IHDR(png, info, &width_, &height_, &bit_depth_, &color_type_, &interlace_type_, &compression_type_, &filter_method_);

    unsigned int row_bytes = png_get_rowbytes(png, info);
    if (data_ != NULL)
    {
        delete[] data_;
    }
    data_ = new unsigned char[row_bytes * height_];

    png_bytepp rows = png_get_rows(png, info);
    for (int i = 0; i < height_; ++i)
    {
        memcpy(data_ + (row_bytes * i), rows[i], row_bytes);
    }

    png_destroy_read_struct(&png, &info, NULL);
}

PNG::~PNG()
{
    if (data_) delete[] data_;
}

unsigned int PNG::get_width()
{
    return width_;
}

unsigned int PNG::get_height()
{
    return height_;
}

unsigned char* PNG::get_data()
{
    return data_;
}

bool PNG::has_alpha()
{
    if (color_type_ == PNG_COLOR_TYPE_RGBA)
    {
        return true;
    }
    return false;
}
