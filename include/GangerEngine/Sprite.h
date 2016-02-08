
#pragma once

#include <GangerEngine/Vertex.h>
#include <GangerEngine/GLTexture.h>

#include <GL/glew.h>
#include <string>

namespace GangerEngine
{
    class Sprite
    {
    public:
        Sprite ();
        ~Sprite ();

        void Init (float x, float y, float width, float height, std::string texturePath);
        void Draw ();

    private:
        void SetSpriteCords (float x, float y, float width, float height, Vertex* vertex);
        void SetColor (Vertex* vertex, int* length);

        template<typename T, int size>
        int GetArrLength (T (&)[size]){ return size; }

        float m_x, m_y, m_width, m_height;
        GLuint m_vboID;
        GLTexture m_texture;
    };
}
