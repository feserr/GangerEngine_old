
#pragma once

#include "GLTexture.h"

#include <string>

class ImageLoader
{
public:
    GLTexture LoadPNG (std::string filePath);
};

