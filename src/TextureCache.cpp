
#include <GangerEngine/TextureCache.h>
#include <GangerEngine/ImageLoader.h>

#include <stdio.h>

namespace GangerEngine
{
    TextureCache::TextureCache ()
    {
    }


    TextureCache::~TextureCache ()
    {
    }

    GLTexture TextureCache::GetTexture (std::string texturePath)
    {
        // Look up the texture in the map
        auto mit = _textureMap.find (texturePath);

        if (mit == _textureMap.end ()) // Check if it is not in the map
        {
            // Load the texture
            GLTexture newTexture = ImageLoader::LoadPNG (texturePath);

            // Insert it to the map
            _textureMap.insert (std::make_pair (texturePath, newTexture));

            printf ("Loaded texture.\n");
            return newTexture;
        }

        printf ("Used cached texture.\n");
        return mit->second;
    }
}
