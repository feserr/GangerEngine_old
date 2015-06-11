
#include <GangerEngine/Camera2D.h>

namespace GangerEngine
{
    Camera2D::Camera2D() : m_position(0.0f, 0.0f), m_cameraMatrix(1.0f), m_orthoMatrix(1.0f),
        m_scale(1.0f), m_screenWidth(500), m_screenHeight(500), m_matrixUpdate(true)
    {
    }


    Camera2D::~Camera2D()
    {
    }

    void Camera2D::Init(int screenWith, int screenHeight)
    {
        m_screenWidth = screenWith;
        m_screenHeight = screenHeight;
        m_orthoMatrix = glm::ortho(0.0f, (float)m_screenWidth,
            0.0f, (float)m_screenHeight);
    }

    void Camera2D::Update()
    {
        if(m_matrixUpdate)
        {
            // Translate the camera
            glm::vec3 translate(-m_position.x + m_screenWidth / 2, -m_position.y + m_screenHeight / 2,
                0.0f);
            m_cameraMatrix = glm::translate(m_orthoMatrix, translate);

            // Scale the camera
            glm::vec3 scale(m_scale, m_scale, 0.0f);
            m_cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * m_cameraMatrix;

            m_matrixUpdate = false;
        }
    }

    bool Camera2D::IsBoxInView(const glm::vec2& position, const glm::vec2 dimension)
    {
        glm::vec2 scaledScreenDimension = glm::vec2(m_screenWidth, m_screenHeight) / (m_scale * 2.0f);

        // The minimum distance before a collision occurs
        const float MIN_DISTANCE_X = dimension.x / 2.0f + scaledScreenDimension.x / 2.0f;
        const float MIN_DISTANCE_Y = dimension.y / 2.0f + scaledScreenDimension.y / 2.0f;

        // Center position of the parameters
        glm::vec2 centerPos = position + dimension / 2.0f;
        // Center position of the camera
        glm::vec2 centerCameraPos = m_position;
        // Vector from the input to the camera
        glm::vec2 distVec = centerPos - centerCameraPos;

        // Get the depth of the collision
        float xDepth = MIN_DISTANCE_X  - abs((int)distVec.x);
        float yDepth = MIN_DISTANCE_Y - abs((int)distVec.y);

        // If either the depths are > 0, then we collided
        if (xDepth > 0 && yDepth > 0)
            return true; // There was a collision

        return false;
    }

    glm::vec2 Camera2D::ConvertScreenToWorld(glm::vec2 screenCoords)
    {
        // Invert Y coordinate
        screenCoords.y = m_screenHeight - screenCoords.y;
        // Make it that 0 is the center
        screenCoords -= glm::vec2(m_screenWidth / 2, m_screenHeight / 2);
        // Scale the coordinates
        screenCoords /= m_scale;
        // Translate with the camera position
        screenCoords += m_position;

        return screenCoords;
    }
}