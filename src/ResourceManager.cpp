
#include <GangerEngine/ResourceManager.h>

namespace GangerEngine
{
    // Declare the static variable
    TextureCache ResourceManager::m_textureCache;

    GLTexture ResourceManager::GetTexture (std::string texturePath)
    {
        return m_textureCache.GetTexture (texturePath);
    }
}
