#ifndef AY_ANDROID_ASSET
#define AY_ANDROID_ASSET

#include <string>

class AndroidAsset
{
public:
    explicit AndroidAsset(std::string name);
    virtual ~AndroidAsset();

    bool Open();
    bool Read(void* pBuffer, const unsigned int bytesToRead, size_t& bytesRead);
    bool Close();
    unsigned int Length() const;

public:
    // getter
    std::string getName() const { return m_name; }

public:
    static void setAssetManager(AAssetManager* val) { m_pAssetManager = val; }

private:
    static AAssetManager* m_pAssetManager;

private:
    std::string m_name;

};

#endif