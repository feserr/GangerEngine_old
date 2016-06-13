// Camera2D.h

#pragma once

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>

namespace GangerEngine
{
    /// Camera class for 2D games
    class Camera2D
    {
    public:
        Camera2D();
        ~Camera2D();

        /// Sets up the orthographic matrix and screen dimensions
        void Init(int screenWith, int screenHeight);

        /// Updates the camera matrix
        void Update();

        bool IsBoxInView(const glm::vec2& position, const glm::vec2 dimension);

        glm::vec2 ConvertScreenToWorld(glm::vec2 screenCoords);

        // Setters
        /// Set the position of the camera
        void SetPosition(const glm::vec2& position) { m_position = position; m_matrixUpdate = true; }
        /// Set the scale of the camera
        void SetScale(float scale) { m_scale = scale; m_matrixUpdate = true; }

        // Getters
        /// Return the position of the camera
        glm::vec2 GetPosition() { return m_position; }
        /// Return the scale of the camera
        float GetScale() { return m_scale; }
        /// Return the camera matrix
        glm::mat4 GetCameraMatrix() { return m_cameraMatrix; }

    private:
        glm::vec2 m_position;
        glm::mat4 m_cameraMatrix;
        glm::mat4 m_orthoMatrix;
        float m_scale;

        int m_screenWidth, m_screenHeight;

        bool m_matrixUpdate;
    };
}
