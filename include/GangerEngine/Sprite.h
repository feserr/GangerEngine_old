// Sprite.h

#pragma once

#include <string>
#include <GL/glew.h>

#include <GangerEngine/Vertex.h>
#include <GangerEngine/GLTexture.h>

namespace GangerEngine
{
    /// A 2D quad that can be rendered to the screen
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
