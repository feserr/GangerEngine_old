/*
    Copyright [2016] [Ganger Games]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <GangerEngine/ImageLoader.h>
#include <GangerEngine/picoPNG.h>
#include <GangerEngine/IOManager.h>
#include <GangerEngine/GangerErrors.h>

#include <string>
#include <vector>

namespace GangerEngine {
    GLTexture ImageLoader::LoadPNG(std::string filePath) {
        // Create a GLTexture and initialize all its fields to 0
        GLTexture texture = {};

        // This is the input data to decodePNG, which we load from a file
        std::vector<unsigned char> in;
        // This is the output data from decodePNG, which is the pixel data for
        // our texture
        std::vector<unsigned char> out;

        unsigned long width, height;

        // Read in the image file contents into a buffer
        if (IOManager::ReadFileToBuffer(filePath, in) == false) {
            FatalError("Failed to load PNG file to buffer!");
        }

        // Decode the .png format into an array of pixels
        int errorCode = DecodePNG(out, width, height, &(in[0]), in.size());
        if (errorCode != 0) {
            FatalError("decodePNG failed with error: " +
                std::to_string(errorCode));
        }

        // Generate the openGL texture object
        glGenTextures(1, &(texture.id));

        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, texture.id);
        // Upload the pixels to the texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

        // Set some texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
            GL_LINEAR_MIPMAP_LINEAR);

        // Generate the mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);

        texture.width = width;
        texture.height = height;
        texture.filePath = filePath;

        // Return a copy of the texture data
        return texture;
    }
}  // namespace GangerEngine
