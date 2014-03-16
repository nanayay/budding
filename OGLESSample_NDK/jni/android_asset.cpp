#include "android_asset.h"

AndroidAsset::m_pAssetManager = NULL;

AndroidAsset::AndroidAsset(std::string name)
    : m_name(name)
{
    assert(m_pAssetManager != NULL);
}

AndroidAsset::~AndroidAsset()
{
    Close();
}

bool AndroidAsset::Open()
{
    // TODO Here
    // add body for android assets
}

bool AndroidAsset::Read(void* pBuffer, const unsigned int bytesToRead, size_t& bytesRead)
{

}

bool AndroidAsset::Close()
{

}

unsigned int AndroidAsset::Length() const
{

}
