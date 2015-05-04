
#pragma once

#include <TextureCache.h>

#include <string>

class ResourceManager
{
public:
    //! Wrapper GetTexture of TextureCache
    static GLTexture GetTexture (std::string texturePath);

private:
    static TextureCache _textureCache;
};

