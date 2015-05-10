
#pragma once

#include <GangerEngine\Vertex.h>

#include <GL\glew.h>
#include <GLM\glm.hpp>

#include <vector>

namespace GangerEngine
{
    enum GlyphSortType
    {
        NONE,
        FRONT_TO_FRONT,
        BACK_TO_FRONT,
        TEXTURE
    };

    struct Glyph
    {
        GLuint texture;
        float depth;

        Vertex topLeft;
        Vertex bottonLeft;
        Vertex topRight;
        Vertex bottonRight;
    };   

    class SpriteBatch
    {
    public:
        SpriteBatch();
        ~SpriteBatch();

        void Init();

        void Begin(GlyphSortType glyphSortType = TEXTURE);
        void End();

        void Draw(const glm::vec4& destRect, const glm::vec4& uvRect, const GLuint& texture,
            const float& depth, const Color& color);

        void RenderBatch(); 

    private:
        void CreateVertexArray();
        void SortGlyphs();

        static bool CompareBackToFront(Glyph* a, Glyph* b);
        static bool CompareFrontToBack(Glyph* a, Glyph* b);
        static bool CompareTexture(Glyph* a, Glyph* b);

        GLuint _vbo;
        GLuint _vao;

        GlyphSortType _glyphSortType;

        std::vector<Glyph*> _glyphs;
    };
}
