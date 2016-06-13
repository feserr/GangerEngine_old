// TextureCache.h

#pragma once

#include <GangerEngine/GLTexture.h>

#include <map>
#include <string>

namespace GangerEngine
{
    //This caches the textures so that multiple sprites can use the same textures
    class TextureCache
    {
    public:
        TextureCache();
        ~TextureCache();

        /// Returns the existing texture or create a new one
        GLTexture GetTexture(std::string filePath);

    private:
        std::map<std::string, GLTexture> m_textureMap;
    };
}
