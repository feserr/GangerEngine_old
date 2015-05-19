
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
        _renderBatches.clear();

        for(unsigned int i = 0; i < _glyphs.size(); i++)
        {
            delete _glyphs[i];
        }

        _glyphs.clear();
    }
    
    void SpriteBatch::End()
    {
        SortGlyphs();
        CreateRenderBatches();
    }

    void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
        const GLuint& texture, const float& depth, const Color& color)
    {
        Glyph* glyph = new Glyph();
        glyph->texture = texture;
        glyph->depth = depth;

        
        glyph->topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
        glyph->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);
        glyph->topLeft.color = color;

        glyph->bottonLeft.SetPosition(destRect.x, destRect.y);
        glyph->bottonLeft.SetUV(uvRect.x, uvRect.y);
        glyph->bottonLeft.color = color;

        glyph->bottonRight.SetPosition(destRect.x + destRect.z, destRect.y);
        glyph->bottonRight.SetUV(uvRect.x + uvRect.z, uvRect.y);
        glyph->bottonRight.color = color;

        glyph->topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
        glyph->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
        glyph->topRight.color = color;

        _glyphs.push_back(glyph);
    }

    void SpriteBatch::RenderBatch()
    {
        // Bind our VAO. This sets up the OpenGL state we need, including the 
        // vertex attribute pointers and it binds the VBO
        glBindVertexArray(_vao);

        for(unsigned int i = 0; i < _renderBatches.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

            glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
        }

        glBindVertexArray(0);
    }

    void SpriteBatch::CreateRenderBatches()
    {
        if(_glyphs.empty())
            return;

        // This will store all the vertices's that we need to upload
        std::vector<Vertex> vertices;

        // Allocate all the memory we need for the vertices's we will add
        vertices.resize(_glyphs.size() * 6);

        int offset = 0;
        int currentVertex = 0;

        // Create a new object and push it back
        _renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);

        // Add the first batch
        vertices[currentVertex++] = _glyphs[0]->topLeft;
        vertices[currentVertex++] = _glyphs[0]->bottonLeft;
        vertices[currentVertex++] = _glyphs[0]->bottonRight;
        vertices[currentVertex++] = _glyphs[0]->bottonRight;
        vertices[currentVertex++] = _glyphs[0]->topRight;
        vertices[currentVertex++] = _glyphs[0]->topLeft;
        
        offset += 6;

        // Add all the rest of the glyphs
        for(unsigned int currentGlyph = 1; currentGlyph < _glyphs.size(); currentGlyph++)
        {
            // Check if the current glyph can be part of the current batch
            if(_glyphs[currentGlyph]->texture != _glyphs[currentGlyph - 1]->texture)
            {
                // Create a new batch
                _renderBatches.emplace_back(offset, 6, _glyphs[currentGlyph]->texture);
            }
            else
            {
                // If it is part of the current batch then increase the numVertices
                _renderBatches.back().numVertices += 6;
            }

            vertices[currentVertex++] = _glyphs[currentGlyph]->topLeft;
            vertices[currentVertex++] = _glyphs[currentGlyph]->bottonLeft;
            vertices[currentVertex++] = _glyphs[currentGlyph]->bottonRight;
            vertices[currentVertex++] = _glyphs[currentGlyph]->bottonRight;
            vertices[currentVertex++] = _glyphs[currentGlyph]->topRight;
            vertices[currentVertex++] = _glyphs[currentGlyph]->topLeft;

            offset += 6;
        }

        // Bind our VBO
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // Orphan the buffer (for speed) because it overwrite the current data
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        // Upload the data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        // Unbind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
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
        return (a->depth > b->depth);
    }

    bool SpriteBatch::CompareFrontToBack(Glyph* a, Glyph* b)
    {
        return (a->depth < b->depth);
    }

    bool SpriteBatch::CompareTexture(Glyph* a, Glyph* b)
    {
        return (a->texture < b->texture);
    }
}
