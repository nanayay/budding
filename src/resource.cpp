#include <sys/stat.h>
#include "resource.h"
#include "log.h"
#include "type_defines.h"

ReadResource::ReadResource(const std::string path)
    : m_path(path)
{
}

ReadResource::~ReadResource()
{
}

ReadFile::ReadFile(const std::string file_path)
    : ReadResource(file_path),
      #if 1
      m_pFileInputStream(NULL),
      #elif 0
      m_FileInputStream(),
      #else
      m_pFile(NULL),
      #endif
      m_Length(0)
{
    // todo notebook, only the class itself own variables can be in init list, others like the super class's, can not be in init list: on the super class's ctor func's argument list, or in this class' ctor's body
    // todo, notebook here, copy ctor is also a ctor, not like assign operator, copy ctor also have init list, you should better add some super class's ctor there, it is good, although not bring compiler error
}

ReadFile::~ReadFile()
{
    Close();
}

bool ReadFile::Open()
{
    #if 1
    m_pFileInputStream = new std::ifstream(m_path.c_str(), std::ios::in | std::ios::binary);
    #elif 0
    m_FileInputStream.open(m_path.c_str(), std::ios::in | std::ios::binary);
    #else
    m_pFile = fopen(m_path.c_str(), "rb");
    #endif

    #if 1
    if (m_pFileInputStream->is_open() == false)
    #elif 0
    if (m_FileInputStream.is_open() == false)
    #else
    if (!m_pFile)
    #endif
    {
        LOGE("ifstream try to open %s just failed", m_path.c_str());
    }

    // todo, c++11, when support c++11, you can use string as open's first parameter, but not use c_str()
    #if 1
    return *m_pFileInputStream ? true : false;
    #elif 0
    return m_FileInputStream ? true : false;
    #else
    return m_pFile ? true : false;
    #endif
}

size_t ReadFile::Read(void* pDestBuffer, const size_t bytesToRead)
{
    #if 1
    m_pFileInputStream->read((char*)pDestBuffer, bytesToRead);
    size_t result = m_pFileInputStream->gcount();
    #elif 0
    m_FileInputStream.read((char*)pDestBuffer, bytesToRead);
    size_t result = m_FileInputStream.gcount();
    #else
    size_t result = fread(pDestBuffer, 1, bytesToRead, m_pFile);
    #endif

    if (!result)
    {
        LOGE("ifstream try to read %d, but just got %d", bytesToRead, result);
    }

    return result;
}

bool ReadFile::Close()
{
    m_Length = 0;
    #if 1
    m_pFileInputStream->close();
    SAFE_DELETE(m_pFileInputStream);
    #elif 0
    m_FileInputStream.close();
    #else
    fclose(m_pFile);
    #endif
    return true;
}

size_t ReadFile::getLength()
{
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