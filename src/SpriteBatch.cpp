
#include <GangerEngine/SpriteBatch.h>

#include <algorithm>

namespace GangerEngine
{
    Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
                 const GLuint& Texture, const float& Depth, const ColorRGBA8& color) :
        texture(Texture), depth(Depth)
    {
        topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
        topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);
        topLeft.color = color;
        
        bottonLeft.SetPosition(destRect.x, destRect.y);
        bottonLeft.SetUV(uvRect.x, uvRect.y);
        bottonLeft.color = color;
        
        bottonRight.SetPosition(destRect.x + destRect.z, destRect.y);
        bottonRight.SetUV(uvRect.x + uvRect.z, uvRect.y);
        bottonRight.color = color;
        
        topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
        topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
        topRight.color = color;
    }

    Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect,
                 const GLuint& Texture, const float& Depth, const ColorRGBA8& color, float angle) :
    texture(Texture), depth(Depth)
    {
        glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);

        // Get points centered at origin
        glm::vec2 tl(-halfDims.x, halfDims.y);
        glm::vec2 bl(-halfDims.x, -halfDims.y);
        glm::vec2 br(halfDims.x, -halfDims.y);
        glm::vec2 tr(halfDims.x, halfDims.y);

        // Roteta the points
        tl = RotatePoint(tl, angle) + halfDims;
        bl = RotatePoint(bl, angle) + halfDims;
        tr = RotatePoint(tr, angle) + halfDims;
        br = RotatePoint(br, angle) + halfDims;

        topLeft.SetPosition(destRect.x + tl.x, destRect.y + tl.y);
        topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);
        topLeft.color = color;

        bottonLeft.SetPosition(destRect.x + bl.x, destRect.y + bl.y);
        bottonLeft.SetUV(uvRect.x, uvRect.y);
        bottonLeft.color = color;

        bottonRight.SetPosition(destRect.x + br.x, destRect.y + br.y);
        bottonRight.SetUV(uvRect.x + uvRect.z, uvRect.y);
        bottonRight.color = color;

        topRight.SetPosition(destRect.x + tr.x, destRect.y + tr.y);
        topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
        topRight.color = color;
    }

    glm::vec2 Glyph::RotatePoint(glm::vec2 pos, float angle)
    {
        glm::vec2 newv;
        newv.x = pos.x * cosf(angle) - pos.y * sinf(angle);
        newv.y = pos.x * sinf(angle) + pos.y * cosf(angle);

        return newv;
    }


    SpriteBatch::SpriteBatch() : m_vbo(0), m_vao(0)
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
        m_glyphSortType = glyphSortType;
        m_renderBatches.clear();

        m_glyphs.clear();
    }
    
    void SpriteBatch::End()
    {
        // Set up all pointer for fast sorting
        m_glyphsPointer.resize(m_glyphs.size());
        for(int i = 0; i < m_glyphs.size(); i++)
        {
            m_glyphsPointer[i] = &m_glyphs[i];
        }
        
        SortGlyphs();
        CreateRenderBatches();
    }

    void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
        const GLuint& texture, const float& depth, const ColorRGBA8& color)
    {
        m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
    }

    void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                           const GLuint& texture, const float& depth, const ColorRGBA8& color,
                           float angle)
    {
        m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
    }

    void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
                           const GLuint& texture, const float& depth, const ColorRGBA8& color,
                           glm::vec2& dir)
    {
        const glm::vec2 right(1.0f, 0.0f);
        float angle = acosf(glm::dot(right, dir));

        if (dir.y < 0.0f)
            angle = -angle;
        
        m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
    }

    void SpriteBatch::RenderBatch()
    {
        // Bind our VAO. This sets up the OpenGL state we need, including the 
        // vertex attribute pointers and it binds the VBO
        glBindVertexArray(m_vao);

        for(unsigned int i = 0; i < m_renderBatches.size(); i++) {
            glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);

            glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
        }

        glBindVertexArray(0);
    }

    void SpriteBatch::CreateRenderBatches()
    {
        if(m_glyphsPointer.empty())
            return;

        // This will store all the vertices's that we need to upload
        std::vector<Vertex> vertices;

        // Allocate all the memory we need for the vertices's we will add
        vertices.resize(m_glyphsPointer.size() * 6);

        int offset = 0;
        int currentVertex = 0;

        // Create a new object and push it back
        m_renderBatches.emplace_back(offset, 6, m_glyphsPointer[0]->texture);

        // Add the first batch
        vertices[currentVertex++] = m_glyphsPointer[0]->topLeft;
        vertices[currentVertex++] = m_glyphsPointer[0]->bottonLeft;
        vertices[currentVertex++] = m_glyphsPointer[0]->bottonRight;
        vertices[currentVertex++] = m_glyphsPointer[0]->bottonRight;
        vertices[currentVertex++] = m_glyphsPointer[0]->topRight;
        vertices[currentVertex++] = m_glyphsPointer[0]->topLeft;
        
        offset += 6;

        // Add all the rest of the glyphs
        for(unsigned int currentGlyph = 1; currentGlyph < m_glyphsPointer.size(); currentGlyph++)
        {
            // Check if the current glyph can be part of the current batch
            if(m_glyphsPointer[currentGlyph]->texture != m_glyphsPointer[currentGlyph - 1]->texture)
            {
                // Create a new batch
                m_renderBatches.emplace_back(offset, 6, m_glyphsPointer[currentGlyph]->texture);
            }
            else
            {
                // If it is part of the current batch then increase the numVertices
                m_renderBatches.back().numVertices += 6;
            }

            vertices[currentVertex++] = m_glyphsPointer[currentGlyph]->topLeft;
            vertices[currentVertex++] = m_glyphsPointer[currentGlyph]->bottonLeft;
            vertices[currentVertex++] = m_glyphsPointer[currentGlyph]->bottonRight;
            vertices[currentVertex++] = m_glyphsPointer[currentGlyph]->bottonRight;
            vertices[currentVertex++] = m_glyphsPointer[currentGlyph]->topRight;
            vertices[currentVertex++] = m_glyphsPointer[currentGlyph]->topLeft;

            offset += 6;
        }

        // Bind our VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
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
        if(m_vao == 0)
            glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // Generate the vertex buffer object
        if(m_vbo == 0)
            glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

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
        switch(m_glyphSortType)
        {
            case GlyphSortType::BACK_TO_FRONT:
                std::stable_sort(m_glyphsPointer.begin(), m_glyphsPointer.end(), CompareBackToFront);
                break;
            case GlyphSortType::FRONT_TO_FRONT:
                std::stable_sort(m_glyphsPointer.begin(), m_glyphsPointer.end(), CompareFrontToBack);
                break;
            case GlyphSortType::TEXTURE:
                std::stable_sort(m_glyphsPointer.begin(), m_glyphsPointer.end(), CompareTexture);
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
