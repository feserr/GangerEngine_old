
#include <GangerEngine/Camera2D.h>

namespace GangerEngine
{
    Camera2D::Camera2D() : _position(0.0f, 0.0f), _cameraMatrix(1.0f), _orthoMatrix(1.0f),
        _scale(1.0f), _screenWidth(500), _screenHeight(500), _matrixUpdate(true)
    {
    }


    Camera2D::~Camera2D()
    {
    }

    void Camera2D::Init(int screenWith, int screenHeight)
    {
        _screenWidth = screenWith;
        _screenHeight = screenHeight;
        _orthoMatrix = glm::ortho(0.0f, (float)_screenWidth,
            0.0f, (float)_screenHeight);
    }

    void Camera2D::Update()
    {
        if(_matrixUpdate)
        {
            // Translate the camera
            glm::vec3 translate(-_position.x + _screenWidth / 2, -_position.y + _screenHeight / 2,
                0.0f);
            _cameraMatrix = glm::translate(_orthoMatrix, translate);

            // Scale the camera
            glm::vec3 scale(_scale, _scale, 0.0f);
            _cameraMatrix = glm::scale(glm::mat4(1.0f), scale) * _cameraMatrix;

            _matrixUpdate = false;
        }
    }
    
    bool Camera2D::IsBoxInView(const glm::vec2& position, const glm::vec2 dimension)
    {
        glm::vec2 scaledScreenDimension = glm::vec2(_screenWidth, _screenHeight) / (_scale * 2.0f);
        
        // The minimum distance before a collision occurs
        const float MIN_DISTANCE_X = dimension.x / 2.0f + scaledScreenDimension.x / 2.0f;
        const float MIN_DISTANCE_Y = dimension.y / 2.0f + scaledScreenDimension.y / 2.0f;
        
        // Center position of the parameters
        glm::vec2 centerPos = position + dimension / 2.0f;
        // Center position of the camera
        glm::vec2 centerCameraPos = _position;
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
        screenCoords.y = _screenHeight - screenCoords.y;
        // Make it that 0 is the center
        screenCoords -= glm::vec2(_screenWidth / 2, _screenHeight / 2);
        // Scale the coordinates
        screenCoords /= _scale;
        // Translate with the camera position
        screenCoords += _position;

        return screenCoords;
    }
}