
#pragma once

#include <GLM\glm.hpp>
#include <GLM\gtc\matrix_transform.hpp>

namespace GangerEngine
{
    class Camera2D
    {
    public:
        Camera2D();
        ~Camera2D();

        void Init(int screenWith, int screenHeight);

        void Update();

        // Setters
        //! Set the position of the camera
        void SetPosition(glm::vec2& position) { _position = position; _matrixUpdate = true; }
        //! Set the scale of the camera
        void SetScale(float scale) { _scale = scale; _matrixUpdate = true; }

        // Getters
        //! Return the position of the camera
        glm::vec2 GetPosition() { return _position; }
        //! Return the scale of the camera
        float GetScale() { return _scale; }
        //! Return the camera matrix
        glm::mat4 GetCameraMatrix() { return _cameraMatrix; }

    private:
        glm::vec2 _position;
        glm::mat4 _cameraMatrix;
        glm::mat4 _orthoMatrix;
        float _scale;

        int _screenWidth, _screenHeight;

        bool _matrixUpdate;
    };
}
