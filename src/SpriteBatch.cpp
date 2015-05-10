
#include <GangerEngine\SpriteBatch.h>

#include <algorithm>

namespace GangerEngine
{
    SpriteBatch::SpriteBatch() : _vbo(0), _vao(0)
    {
    }


    SpriteBatch::~SpriteBatch()
    {
    }

    void SpriteBatch::Init()
    {
        CreateVertexArray();
    }

    void SpriteBatch::Begin(GlyphSortType glyphSortType /* GlyphShortType::TEXTURE*/)
    {
        _glyphSortType = glyphSortType;
    }
    
    void SpriteBatch::End()
    {
        SortGlyphs();
    }

    void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
        const GLuint& texture, const float& depth, const Color& color)
    {
        Glyph* glyph = new Glyph();
        glyph->texture = texture;
        glyph->depth = depth;

        
        glyph->topLeft.SetPosition(destRect.x, destRect.y + uvRect.w);
        glyph->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);
        glyph->topLeft.color = color;

        glyph->bottonLeft.SetPosition(destRect.x, destRect.y);
        glyph->bottonLeft.SetUV(uvRect.x, uvRect.y);
        glyph->bottonLeft.color = color;

        glyph->topRight.SetPosition(destRect.x + uvRect.z, destRect.y + uvRect.w);
        glyph->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
        glyph->topRight.color = color;

        glyph->bottonRight.SetPosition(destRect.x + uvRect.z, destRect.y);
        glyph->bottonRight.SetUV(uvRect.x + uvRect.z, uvRect.y);
        glyph->bottonRight.color = color;

        _glyphs.push_back(glyph);
    }

    void SpriteBatch::RenderBatch()
    {
    }

    void SpriteBatch::CreateVertexArray()
    {
        // Generate the vertex array object 
        if(_vao == 0)
            glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);

        // Generate the vertex buffer object
        if(_vbo == 0)
            glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        /* Tell OpenGL that we want to use the first
        * attribute array. We only need one array right
        * now since we are only using position */
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // This is the position attribute pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, position));

        // This is the UV attribute pointer
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
            (void*)offsetof(Vertex, uv));

        // This is the color attribute pointer
        glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
            (void*)offsetof(Vertex, color));

        glBindVertexArray(0);
    }

    void SpriteBatch::SortGlyphs()
    {
        switch(_glyphSortType)
        {
            case GlyphSortType::BACK_TO_FRONT:
                std::stable_sort(_glyphs.begin(), _glyphs.end(), CompareBackToFront);
                break;
            case GlyphSortType::FRONT_TO_FRONT:
                std::stable_sort(_glyphs.begin(), _glyphs.end(), CompareFrontToBack);
                break;
            case GlyphSortType::TEXTURE:
                std::stable_sort(_glyphs.begin(), _glyphs.end(), CompareTexture);
                break;
            default:
                break;
        }
        
    }

    bool SpriteBatch::CompareBackToFront(Glyph* a, Glyph* b)
    {
        return a->depth > b->depth;
    }

    bool SpriteBatch::CompareFrontToBack(Glyph* a, Glyph* b)
    {
        return a->depth < b->depth;
    }

    bool SpriteBatch::CompareTexture(Glyph* a, Glyph* b)
    {
        return a->texture < b->texture;
    }
}
