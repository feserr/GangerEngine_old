
#include <GangerEngine/InputManager.h>

namespace GangerEngine
{
    InputManager::InputManager() : m_mouseCoords(0,0)
    {
    }

    InputManager::~InputManager()
    {
    }

    void InputManager::Update()
    {
        for(auto& it : m_keyMap)
        {
            m_previousKeyMap[it.first] = it.second;
        }
    }

    void InputManager::PressKey(unsigned int keyID)
    {
        m_keyMap[keyID] = true;
    }

    void InputManager::ReleaseKey(unsigned int keyID)
    {
        m_keyMap[keyID] = false;
    }

    void InputManager::SetMouseCoords(float x, float y)
    {
        m_mouseCoords.x = x;
        m_mouseCoords.y = y;
    }

    bool InputManager::isKeyDown(unsigned int keyID)
    {
        auto it = m_keyMap.find(keyID);

        if(it != m_keyMap.end())
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
        auto it = m_previousKeyMap.find(keyID);

        if(it != m_previousKeyMap .end())
            return it->second;

        return false;
    }
}
