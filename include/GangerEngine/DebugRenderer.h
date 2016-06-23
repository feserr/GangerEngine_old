/*
    Copyright [2016] [Ganger Games]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _DEBUGRENDERER_H_
#define _DEBUGRENDERER_H_

#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/Vertex.h>

#include <glm/glm.hpp>
#include <vector>

namespace GangerEngine {
class DebugRenderer {
 public:
    DebugRenderer();
    ~DebugRenderer();

    void Init();
    void End();
    void DrawLine(const glm::vec2& a, const glm::vec2& b,
        const ColorRGBA8& color);
    void DrawBox(const glm::vec4& destRect, const ColorRGBA8& color,
        float angle);
    void DrawCircle(const glm::vec2& center, const ColorRGBA8& color,
        float radius);
    void Render(const glm::mat4& projectionMatrix, float lineWidth);
    void Dispose();

    struct DebugVertex {
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
}  // namespace GangerEngine

#endif  // _DEBUGRENDERER_H_
