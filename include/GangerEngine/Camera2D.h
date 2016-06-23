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

#ifndef _CAMERA2D_H_
#define _CAMERA2D_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GangerEngine {
// Camera class for 2D games
class Camera2D {
 public:
    Camera2D();
    ~Camera2D();

    /// Sets up the orthographic matrix and screen dimensions
    void Init(int screenWidth, int screenHeight);

    /// Updates the camera matrix if needed
    void Update();

    glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords);

    bool IsBoxInView(const glm::vec2& position, const glm::vec2& dimensions);

    void OffsetPosition(const glm::vec2& offset) {
        m_position += offset;
        m_needsMatrixUpdate = true;
    }
    void OffsetScale(float offset) {
        m_scale += offset;
        if (m_scale < 0.001f)
            m_scale = 0.001f;
        m_needsMatrixUpdate = true;
    }

    // Setters
    void SetPosition(const glm::vec2& newPosition) {
        m_position = newPosition;
        m_needsMatrixUpdate = true;
    }
    void SetScale(float newScale) {
        m_scale = newScale;
    m_needsMatrixUpdate = true;
    }

    // Getters
    const glm::vec2& GetPosition() const { return m_position; }
    float GetScale() const { return m_scale; }
    const glm::mat4& GetCameraMatrix() const { return m_cameraMatrix; }
    float GetAspectRatio() const {
        return static_cast<float>(m_screenWidth) /
            static_cast<float>(m_screenHeight);
    }

 private:
    int m_screenWidth, m_screenHeight;
    bool m_needsMatrixUpdate;
    float m_scale;
    glm::vec2 m_position;
    glm::mat4 m_cameraMatrix;
    glm::mat4 m_orthoMatrix;
};
}  // namespace GangerEngine

#endif  // _CAMERA2D_H_
