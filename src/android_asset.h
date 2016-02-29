#ifndef AY_ANDROID_ASSET
#define AY_ANDROID_ASSET

#include <string>
#include <android/asset_manager.h>

#include "resource.h"

class AndroidAsset
    : public ReadResource
{
public:
    explicit AndroidAsset(std::string path, AAssetManager* assetManager = NULL);
    virtual ~AndroidAsset();

    virtual bool Open();
    virtual size_t Read(void* pDestBuffer, const size_t bytesToRead);
    virtual bool Close();

    size_t getLength() const;

public:
    static void setAssetManager(AAssetManager* val) { m_pAssetManager = val; }

private:
    static AAssetManager* m_pAssetManager;

private:
    AAsset* m_pAsset;

};

#endif