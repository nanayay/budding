#include <sys/stat.h>
#include "resource.h"

ReadResource::ReadResource(const std::string path)
    : m_path(path)
{
}

ReadResource::~ReadResource()
{
}

ReadFile::ReadFile(const std::string file_path)
    : ReadResource(file_path),
      m_FileInputStream(),
      m_Length(0)
{
    // todo notebook, only the class itself own variables can be in init list, others like the super class's, can not be in init list: on the super class's ctor func's argument list, or in this class' ctor's body
}

ReadFile::~ReadFile()
{
    Close();
}

ReadFile::ReadFile(ReadFile& copy_)
    : ReadResource(copy_.getPath())
{
    copy_.Close();
    // m_path = copy_.getPath();
    // todo, notebook here, copy ctor is also a ctor, not like assign operator, copy ctor also have init list, you should better add some super class's ctor there, it is good, although not bring compiler error
    m_Length = 0;
}

bool ReadFile::Open()
{
    m_FileInputStream.open(m_path.c_str(), std::ios::in | std::ios::binary);
    // todo, c++11, when support c++11, you can use string as open's first parameter, but not use c_str()
    return m_FileInputStream ? true : false;
    // todo, notebook, use object as the condition check, how to override operator bool, etc.
}

size_t ReadFile::Read(void* pDestBuffer, const size_t bytesToRead)
{
    m_FileInputStream.read((char*)pDestBuffer, bytesToRead);
    return m_FileInputStream.gcount();
    // todo, notebook, how to get the just readed bytes
}

bool ReadFile::Close()
{
    m_FileInputStream.close();
    m_Length = 0;
    return true;
}

size_t ReadFile::getLength()
{
    // todo, notebook, how to read the file's size, without using std headers
    struct stat filestatus;
    if (stat(m_path.c_str(), &filestatus) >= 0)
    {
        return filestatus.st_size;
    }
    else
    {
        return -1;
    }
}