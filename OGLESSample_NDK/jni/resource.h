#ifndef AY_RESOURCE_H
#define AY_RESOURCE_H

#include <string>
#include <fstream>

class ReadResource
{
public:
    explicit ReadResource(const std::string path);
    virtual ~ReadResource();

    virtual bool Open() = 0;
    virtual size_t Read(void* pDestBuffer, const size_t bytesToRead) = 0;
    virtual bool Close() = 0;

    std::string getPath() const { return m_path; }

protected:
    std::string m_path;
};

class ReadFile
    : public ReadResource
{
public:
    explicit ReadFile(const std::string file_path);
    virtual ~ReadFile();

    virtual bool Open();
    virtual size_t Read(void* pDestBuffer, const size_t bytesToRead);
    virtual bool Close();

    size_t getLength();

private:
    std::ifstream m_FileInputStream;
    size_t m_Length;

    // todo notebook, all file stream should not be copy-able
    explicit ReadFile(ReadFile& copy_);
    ReadFile& operator=(ReadFile& assign_);

};

#endif