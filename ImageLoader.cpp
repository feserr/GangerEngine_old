
#include "ImageLoader.h"
#include "PicoPNG.h"
#include "IOManager.h"
#include "Errors.h"

#include <vector>

GLTexture ImageLoader::LoadPNG (std::string filePath)
{
    // Initialize all parameters to zero
    GLTexture texture = {};

    std::vector<unsigned char> out;
    std::vector<unsigned char> in;

    unsigned long width, height;

    // Read the image file into a buffer
    if (IOManager::readFileToBuffer (filePath, in))
        FatalError ("Failed to load PNG file to buffer.");


    // Decode the png into an array of pixels
    int error = decodePNG (out, width, height, &in[0], in.size);
    if (error != 0)
    {
        FatalError ("DecodePNG failed with error: " + std::to_string(error) + ".");
    }

    // Generate the openGL textures
    glGenTextures (1, &texture.id);

    // Bind the texture object
    glBindTexture (GL_TEXTURE_2D, texture.id);

    // Upload the pixels to the texture
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        &(out[0]));

    // Set texture parameters
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Generate the mipmap texture
    glGenerateMipmap (GL_TEXTURE_2D);

    // Unbind the texture
    glBindTexture (GL_TEXTURE_2D, 0);

    // Return a copy of the texture data
    return texture;
}
