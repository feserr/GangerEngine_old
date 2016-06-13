// ResourceManager.h

#pragma once

#include <GangerEngine/TextureCache.h>

#include <string>

namespace GangerEngine
{
    // This is a way for us to access all our resources, such as
    // Models or textures.
    class ResourceManager
    {
    public:
        /// Wrapper GetTexture of TextureCache
        static GLTexture GetTexture(std::string texturePath);

    private:
        static TextureCache m_textureCache;
    };
}
