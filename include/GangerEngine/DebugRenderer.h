/*
    Copyright [2016] [Elías Serrano]

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
/// A class to draw debug graphics.
class DebugRenderer {
 public:
    /// Default constructor.
    DebugRenderer();
    /// Default destructor.
    ~DebugRenderer();

    /// Initialize the debug renderer.
    void Init();

    /// End the debug renderer.
    void End();

    /**
     * \brief      Draws a line.
     *
     * \param[in]  a      Start position
     * \param[in]  b      End position
     * \param[in]  color  The color
     */
    void DrawLine(const glm::vec2& a, const glm::vec2& b,
        const ColorRGBA8& color);

    /**
     * \brief      Draws a box.
     *
     * \param[in]  destRect  The destination rectangle
     * \param[in]  color     The color
     * \param[in]  angle     The angle
     */
    void DrawBox(const glm::vec4& destRect, const ColorRGBA8& color,
        float angle);

    /**
     * \brief      Draws a circle.
     *
     * \param[in]  center  The center
     * \param[in]  color   The color
     * \param[in]  radius  The radius
     */
    void DrawCircle(const glm::vec2& center, const ColorRGBA8& color,
        float radius);

    /**
     * \brief      Renders the debug graphics.
     *
     * \param[in]  projectionMatrix  The projection matrix
     * \param[in]  lineWidth         The line width
     */
    void Render(const glm::mat4& projectionMatrix, float lineWidth);

    /// Terminates the debug renderer.
    void Dispose();

    /// Struct for vertex.
    struct DebugVertex {
        glm::vec2 position;  ///< The position
        GangerEngine::ColorRGBA8 color; ///< The color
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
