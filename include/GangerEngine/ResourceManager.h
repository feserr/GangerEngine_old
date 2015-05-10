
#pragma once

#include <GangerEngine/TextureCache.h>

#include <string>

namespace GangerEngine
{
    class ResourceManager
    {
    public:
        //! Wrapper GetTexture of TextureCache
        static GLTexture GetTexture (std::string texturePath);

    private:
        static TextureCache _textureCache;
    };
}
