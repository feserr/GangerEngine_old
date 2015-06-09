
#pragma once

#include <GLM/glm.hpp>
#include <unordered_map>

namespace GangerEngine
{
    class InputManager
    {
    public:
        InputManager();
        ~InputManager();
        
        //! Updates the previous keys pressed
        void Update();

        void PressKey(unsigned int keyID);
        void ReleaseKey(unsigned int keyID);

        void SetMouseCoords(float x, float y);

        //! Returns the status of the key
        bool isKeyDown(unsigned int keyID);
        
        //! Check if the key has been pressed
        bool isKeyPressed(unsigned int keyID);

        // Getters
        glm::vec2 GetMouseCoords() const { return _mouseCoords; }

    private:
        bool wasKeyDown(unsigned int keyID);
        
        std::unordered_map<unsigned int, bool> _keyMap;
        std::unordered_map<unsigned int, bool> _previousKeyMap;
        glm::vec2 _mouseCoords;
    };
}