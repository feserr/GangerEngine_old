
#include <GangerEngine/Sprite.h>
#include <GangerEngine/ImageLoader.h>
#include <GangerEngine/ResourceManager.h>

#include <cstddef>
#include <stdio.h>

namespace GangerEngine
{
    Sprite::Sprite () : _vboID (0)
    {
    }

    Sprite::~Sprite ()
    {
        if (_vboID != 0)
        {
            glDeleteBuffers (1, &_vboID);
        }
    }

    void Sprite::Init (float x, float y, float width, float height, std::string texturePath)
    {
        _x = x;
        _y = y;
        _width = width;
        _height = height;
        _texture = ResourceManager::GetTexture (texturePath);

        if (_vboID == 0)
        {
            glGenBuffers (1, &_vboID);
        }

        Vertex vertexData[6];
        SetSpriteCords (_x, _y, _width, _height, vertexData);
        int vertexLength = GetArrLength (vertexData);
        SetColor (vertexData, &vertexLength);

        // Tell OpenGL to bind our vertex buffer object
        glBindBuffer (GL_ARRAY_BUFFER, _vboID);

        // Upload the data to the GPU
        glBufferData (GL_ARRAY_BUFFER, sizeof (vertexData), vertexData, GL_STATIC_DRAW);

        // Unbind the buffer (optional)
        glBindBuffer (GL_ARRAY_BUFFER, 0);
    }

    void Sprite::Draw ()
    {
        glBindTexture (GL_TEXTURE_2D, _texture.id);

        // Bind the buffer object
        glBindBuffer (GL_ARRAY_BUFFER, _vboID);

        /* Tell OpenGL that we want to use the first
         * attribute array. We only need one array right
         * now since we are only using position */
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // This is the position attribute pointer
        glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex),
            (void*)offsetof (Vertex, position));

        // This is the UV attribute pointer
        glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex),
            (void*)offsetof (Vertex, uv));

        // This is the color attribute pointer
        glVertexAttribPointer (2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof (Vertex),
            (void*)offsetof (Vertex, color));

        // Draw the 6 vertices's to the screen
        glDrawArrays (GL_TRIANGLES, 0, 6);

        // Disable the vertex attribute array
        glDisableVertexAttribArray (0);

        // Unbind the VBO
        glBindBuffer (GL_ARRAY_BUFFER, 0);
    }

    void Sprite::SetSpriteCords (float x, float y, float width, float height, Vertex* vertex)
    {
        // first triangle
        vertex[0].SetPosition (x + width, y + height);
        vertex[0].SetUV (1.0f, 1.0f);

        vertex[1].SetPosition (x, y + height);
        vertex[1].SetUV (0.0f, 1.0f);

        vertex[2].SetPosition (x, y);
        vertex[2].SetUV (0.0f, 0.0f);

        // second triangle
        vertex[3].SetPosition (x, y);
        vertex[3].SetUV (0.0f, 0.0f);

        vertex[4].SetPosition (x + width, y);
        vertex[4].SetUV (1.0f, 0.0f);

        vertex[5].SetPosition (x + width, y + height);
        vertex[5].SetUV (1.0f, 1.0f);
    }

    void Sprite::SetColor (Vertex* vertex, int* length)
    {
        for (int i = 0; i < *length; i++)
        {
            vertex[i].SetColor (255, 255, 255, 255);
        }

        // Extra colors
        //vertex[1].SetColor (0, 0, 255, 255);

        //vertex[4].SetColor (0, 255, 0, 255);
    }
}
