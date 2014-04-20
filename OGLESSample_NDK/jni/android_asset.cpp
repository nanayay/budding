#include <assert.h>
#include "android_asset.h"
#include "log.h"

AAssetManager* AndroidAsset::m_pAssetManager = NULL;

AndroidAsset::AndroidAsset(std::string path, AAssetManager* assetManager)
    : ReadResource(path),
      m_pAsset(NULL)
{
    if (assetManager != NULL)
    {
        AndroidAsset::m_pAssetManager = assetManager;
    }
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

    if (!m_pAsset)
    {
        LOGE("AAssetManager_open try to open %s just failed", m_path.c_str());
    }

    return m_pAsset ? true : false;
}

size_t AndroidAsset::Read(void* pDestBuffer, const size_t bytesToRead)
{
    size_t result = AAsset_read(m_pAsset, pDestBuffer, bytesToRead);

    if (!result)
    {
        LOGE("AAsset_read try to read %d, but just got %d", bytesToRead, result);
    }

    return result;
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

size_t AndroidAsset::getLength() const
{
    return AAsset_getLength(m_pAsset);
}
