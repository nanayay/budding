#include <assert.h>
#include "android_asset.h"

AAssetManager* AndroidAsset::m_pAssetManager = NULL;

AndroidAsset::AndroidAsset(std::string path, AAssetManager* assetManager)
    : ReadResource(path),
      m_pAsset(NULL)
{
    if (assetManager != NULL)
    {
        AndroidAsset::m_pAssetManager = assetManager;
    }
    assert(m_pAssetManager != NULL);
}

AndroidAsset::~AndroidAsset()
{
    Close();
}

bool AndroidAsset::Open()
{
    assert(m_pAssetManager != NULL);
    assert(m_path.length() != 0);
    m_pAsset = AAssetManager_open(m_pAssetManager, m_path.c_str(), AASSET_MODE_UNKNOWN);
    return m_pAsset ? true : false;
}

size_t AndroidAsset::Read(void* pDestBuffer, const size_t bytesToRead)
{
    return AAsset_read(m_pAsset, pDestBuffer, bytesToRead);
}

bool AndroidAsset::Close()
{
    if (m_pAsset)
    {
        AAsset_close(m_pAsset);
        m_pAsset = NULL;
    }
    return true;
}

unsigned int AndroidAsset::Length() const
{
    return AAsset_getLength(m_pAsset);
}
