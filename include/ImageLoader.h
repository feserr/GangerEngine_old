
#pragma once

#include <GLTexture.h>

#include <string>

class ImageLoader
{
public:
    static GLTexture LoadPNG (std::string filePath);
};

