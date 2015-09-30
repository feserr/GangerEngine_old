
#pragma once

#include <GL/glew.h>

struct Position
{
    float x;
    float y;
};

struct Color
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
};

struct UV
{
    float u;
    float v;
};

struct Vertex
{
    //! Position of the vertex.
    Position position;

    //! UV texture coordinates.
    UV uv;

    //! 4 bytes of RGBA color.
    Color color;

    //! Set the position of the vertex
    void SetPosition (float x, float y)
    {
        position.x = x;
        position.y = y;
    }

    //! Set the UV coordinates
    void SetUV (float u, float v)
    {
        uv.u = u;
        uv.v = v;
    }

    //! Set the color
    void SetColor (GLubyte r, GLubyte g, GLubyte b, GLubyte a)
    {
        color.r = r;
        color.g = g;
        color.b = b;
        color.a = a;
    }
};
