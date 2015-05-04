
#include <ResourceManager.h>

// Declare the static variable
TextureCache ResourceManager::_textureCache;

GLTexture ResourceManager::GetTexture (std::string texturePath)
{
    return _textureCache.GetTexture (texturePath);
}