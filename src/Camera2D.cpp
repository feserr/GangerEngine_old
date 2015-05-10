
#include <GangerEngine\Camera2D.h>

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
}