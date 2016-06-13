// DebugRenderer.h

#pragma once

#include <vector>
#include <GLM/glm.hpp>

#include "GLSLProgram.h"
#include "Vertex.h"

namespace GangerEngine
{
    class DebugRenderer
    {
    public:
        DebugRenderer();
        ~DebugRenderer();

        void Init();
        void End();
        void DrawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle);
        void DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
        void Render(const glm::mat4& projectionMatrix, float lineWidth);
        void Dispose();

        struct DebugVertex
        {
            glm::vec2 position;
            GangerEngine::ColorRGBA8 color;
        };

    private:
        GangerEngine::GLSLProgram m_program;
        std::vector<DebugVertex> m_verts;
        std::vector<GLuint> m_indices;
        GLuint m_vbo = 0, m_vao = 0, m_ibo = 0;
        int m_numElements = 0;
    };
}
