
#pragma once

#include <GL/glew.h>

namespace GangerEngine
{
    struct Position
    {
        Position () : x(0.0f), y(0.0f) {}
        Position (float X, float Y) : x(X), y(Y) {}
        float x;
        float y;
    };

    struct ColorRGBA8
    {
        ColorRGBA8() : r(0), g(0), b(0), a(0) {}
        ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
            r(R), g(G), b(B), a(A) {}
        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    };

    struct UV
    {
        UV() : u(0), v(0) {}
        UV(float U, float V) : u(U), v(V) {}
        float u;
        float v;
    };

    struct Vertex
    {
        Vertex() : position(0,0), uv(0,0), color(0,0,0,0) {}
        Vertex(Position p, UV u, ColorRGBA8 c) : position(p), uv(u), color(c) {}

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

        //! Position of the vertex.
        Position position;

        //! UV texture coordinates.
        UV uv;

        //! 4 bytes of RGBA color.
        ColorRGBA8 color;
    };
}
