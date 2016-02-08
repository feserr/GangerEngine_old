
#include <GangerEngine/ImageLoader.h>
#include <GangerEngine/PicoPNG.h>
#include <GangerEngine/IOManager.h>
#include <GangerEngine/GangerErrors.h>

#include <vector>

namespace GangerEngine
{
    GLTexture ImageLoader::LoadPNG (std::string filePath)
    {
        // Initialize all parameters to zero
        GLTexture texture = {};

        //This is the output data from decodePNG, which is the pixel data for our texture
        std::vector<unsigned char> out;

        // This is the input data to decodePNG, which we load from a file
        std::vector<unsigned char> in;

        unsigned long width, height;

        // Read the image file into a buffer
        if (IOManager::ReadFileToBuffer (filePath, in) == false)
            FatalError ("Failed to load PNG file to buffer.");


        // Decode the PNG into an array of pixels
        int error = decodePNG (out, width, height, &in[0], in.size ());
        if (error != 0)
        {
            FatalError ("DecodePNG failed with error: " + std::to_string (error) + ".");
        }

        // Generate the openGL textures
        glGenTextures (1, &texture.id);

        // Bind the texture object
        glBindTexture (GL_TEXTURE_2D, texture.id);

        // Upload the pixels to the texture
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, (int)width, (int)height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
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

        // Apply width and height to the texture
        texture.width = (int)width;
        texture.height = (int)height;

        // Return a copy of the texture data
        return texture;
    }
}
