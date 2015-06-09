
#include <GangerEngine/InputManager.h>

namespace GangerEngine
{
    InputManager::InputManager() : _mouseCoords(0,0)
    {
    }

    InputManager::~InputManager()
    {
    }
    
    void InputManager::Update()
    {
        for(auto& it : _keyMap)
        {
            _previousKeyMap[it.first] = it.second;
        }
    }

    void InputManager::PressKey(unsigned int keyID)
    {
        _keyMap[keyID] = true;
    }

    void InputManager::ReleaseKey(unsigned int keyID)
    {
        _keyMap[keyID] = false;
    }

    void InputManager::SetMouseCoords(float x, float y)
    {
        _mouseCoords.x = x;
        _mouseCoords.y = y;
    }

    bool InputManager::isKeyDown(unsigned int keyID)
    {
        auto it = _keyMap.find(keyID);

        if(it != _keyMap.end())
            return it->second;

        return false;
    }
    
    bool InputManager::isKeyPressed(unsigned int keyID)
    {
        // Check if the key is press at this moment 
        if (isKeyDown(keyID) && !wasKeyDown(keyID))
            return true;
        
        return false;
    }
    
    bool InputManager::wasKeyDown(unsigned int keyID)
    {
        auto it = _previousKeyMap.find(keyID);
        
        if(it != _previousKeyMap .end())
            return it->second;
        
        return false;
    }
}
