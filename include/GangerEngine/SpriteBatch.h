
#pragma once

#include <GangerEngine/Vertex.h>

#include <GL/glew.h>
#include <GLM/glm.hpp>

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

    class Glyph
    {
    public:
        Glyph() {}
        Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
              const GLuint& Texture, const float& Depth, const ColorRGBA8& color);

        Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
              const GLuint& Texture, const float& Depth, const ColorRGBA8& color, float angle);

        GLuint texture;
        float depth;

        Vertex topLeft;
        Vertex bottonLeft;
        Vertex topRight;
        Vertex bottonRight;

    private:
        glm::vec2 RotatePoint(glm::vec2 pos, float angle);
    };

    class RenderBatch
    {
    public:
        RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : offset(offset),
            numVertices(numVertices), texture(texture) {}

        GLuint offset;
        GLuint numVertices;
        GLuint texture;
    };

    class SpriteBatch
    {
    public:
        SpriteBatch();
        ~SpriteBatch();

        void Init();

        void Begin(GlyphSortType glyphSortType = TEXTURE);
        void End();

        void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                  const GLuint& texture,
            const float& depth, const ColorRGBA8& color);

        void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                  const GLuint& texture, const float& depth, const ColorRGBA8& color,
                  float angle);

        void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                  const GLuint& texture, const float& depth, const ColorRGBA8& color,
                  glm::vec2& dir);

        void RenderBatch();

    private:
        void CreateVertexArray();
        void CreateRenderBatches();
        void SortGlyphs();

        static bool CompareBackToFront(Glyph* a, Glyph* b);
        static bool CompareFrontToBack(Glyph* a, Glyph* b);
        static bool CompareTexture(Glyph* a, Glyph* b);

        GLuint m_vbo;
        GLuint m_vao;

        GlyphSortType m_glyphSortType;

        std::vector<Glyph*> m_glyphsPointer; ///< This is for sorting
        std::vector<Glyph> m_glyphs; ///< This is the actual glyphs
        std::vector<GangerEngine::RenderBatch> m_renderBatches;
    };
}
