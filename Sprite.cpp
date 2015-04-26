
#include "Sprite.h"

#include <cstddef>

Sprite::Sprite() : _vboID(0)
{
}

Sprite::~Sprite()
{
    if (_vboID != 0)
    {
        glDeleteBuffers(1, &_vboID);
    }
}

void Sprite::Init(float x, float y, float width, float height)
{
    _x = x;
    _y = y;
    _width = width;
    _height = height;
    
    if (_vboID == 0)
    {
        glGenBuffers(1, &_vboID);
    }

    Vertex vertexData[6];
    SetSpriteCords(_x, _y, _width, _height, vertexData);
    int vertexLength = GetArrLength(vertexData);
    SetColor(vertexData, &vertexLength);

    // Tell opengl to bind our vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);

    // Upload the data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Unbind the buffer (optional)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::Draw()
{
    // Bind the buffer object
    glBindBuffer(GL_ARRAY_BUFFER, _vboID);

    /* Tell opengl that we want to use the first
     * attribute array. We only need one array right
     * now since we are only using position */
    glEnableVertexAttribArray(0);

    // This is the position attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        (void*)offsetof(Vertex, position));
    // This is the color attribute pointer
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
        (void*)offsetof(Vertex, color));

    // Draw the 6 vertices to the screen
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Disable the vertex attrib array
    glDisableVertexAttribArray(0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::SetSpriteCords(float x, float y, float width, float height, Vertex* vertex)
{
    // first triangle
    vertex[0].position.x = y + width;
    vertex[0].position.y = y + height;

    vertex[1].position.x = x;
    vertex[1].position.y = y + height;

    vertex[2].position.x = x;
    vertex[2].position.y = y;

    // second triangle
    vertex[3].position.x = x;
    vertex[3].position.y = y;

    vertex[4].position.x = x + width;
    vertex[4].position.y = y;

    vertex[5].position.x = x + width;
    vertex[5].position.y = y + height;
}

void Sprite::SetColor(Vertex* vertex, int* length)
{
    for (int i = 0; i < *length; i++)
    {
        vertex[i].color.r = 255;
        vertex[i].color.g = 0;
        vertex[i].color.b = 255;
        vertex[i].color.a = 255;
    }

    // Extra colors
    vertex[1].color.r = 0;
    vertex[1].color.g = 0;
    vertex[1].color.b = 255;
    vertex[1].color.a = 255;

    vertex[4].color.r = 0;
    vertex[4].color.g = 255;
    vertex[4].color.b = 0;
    vertex[4].color.a = 255;
}
