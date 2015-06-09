
#pragma once

#include <GangerEngine/GLTexture.h>

#include <string>

namespace GangerEngine
{
    class ImageLoader
    {
    public:
        static GLTexture LoadPNG (std::string filePath);
    };
}
