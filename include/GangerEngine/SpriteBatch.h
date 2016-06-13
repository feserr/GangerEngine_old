// SpriteBatch.h

#pragma once

#include <GangerEngine/Vertex.h>

#include <GL/glew.h>
#include <GLM/glm.hpp>

#include <vector>

namespace GangerEngine
{
    /// Determines how we should sort the glyphs
    enum GlyphSortType
    {
        NONE,
        FRONT_TO_FRONT,
        BACK_TO_FRONT,
        TEXTURE
    };

    /// A glyph is a single quad. These are added via SpriteBatch::draw
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
        /// Rotates a point about (0,0) by angle
        glm::vec2 RotatePoint(glm::vec2 pos, float angle);
    };

    /// Each render batch is used for a single draw call
    class RenderBatch
    {
    public:
        RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) : offset(offset),
            numVertices(numVertices), texture(texture) {}

        GLuint offset;
        GLuint numVertices;
        GLuint texture;
    };

    /// The SpriteBatch class is a more efficient way of drawing sprites
    class SpriteBatch
    {
    public:
        SpriteBatch();
        ~SpriteBatch();

        /// Initializes the spritebatch
        void Init();

        /// Begins the spritebatch
        void Begin(GlyphSortType glyphSortType = TEXTURE);

        /// Ends the spritebatch
        void End();

        /// Adds a glyph to the spritebatch
        void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                  const GLuint& texture,
            const float& depth, const ColorRGBA8& color);
        /// Adds a glyph to the spritebatch with rotation
        void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                  const GLuint& texture, const float& depth, const ColorRGBA8& color,
                  float angle);
        /// Adds a glyph to the spritebatch with rotation
        void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                  const GLuint& texture, const float& depth, const ColorRGBA8& color,
                  glm::vec2& dir);

        /// Renders the entire SpriteBatch to the screen
        void RenderBatch();

    private:
        /// Creates all the needed RenderBatches
        void CreateVertexArray();
        /// Generates our VAO and VBO
        void CreateRenderBatches();
        /// Sorts glyphs according to _sortType
        void SortGlyphs();

        // Comparators used by sortGlyphs()
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
