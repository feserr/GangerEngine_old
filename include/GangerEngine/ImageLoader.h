// ImageLoader.h

#pragma once

#include <GangerEngine/GLTexture.h>

#include <string>

namespace GangerEngine
{
    /// Loads images into GLTextures
    class ImageLoader
    {
    public:
        static GLTexture LoadPNG(std::string filePath);
    };
}
