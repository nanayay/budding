#include <assert.h>
#include "android_asset.h"

AAssetManager* AndroidAsset::m_pAssetManager = NULL;

AndroidAsset::AndroidAsset(std::string name)
    : m_name(name),
      m_pAsset(NULL)
{
    assert(m_pAssetManager != NULL);
}

AndroidAsset::~AndroidAsset()
{
    Close();
}

bool AndroidAsset::Open()
{
    m_pAsset = AAssetManager_open(m_pAssetManager, m_name.c_str(), AASSET_MODE_UNKNOWN);
    return m_pAsset ? true : false;
}

bool AndroidAsset::Read(void* pBuffer, const unsigned int bytesToRead, size_t& bytesRead)
{
    bytesRead = AAsset_read(m_pAsset, pBuffer, bytesToRead);
    return true;
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
