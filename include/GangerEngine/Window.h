
#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <string>

namespace GangerEngine
{
    enum WindowFlags
    {
        INVISIBLE = 0x1,
        FULLSCREEN = 0x2,
        BORDERLESS = 0x4
    };

    class Window
    {
    public:
        Window();
        ~Window();

        int Create(std::string windowName, int screenWidth, int screenHeight,
            unsigned int currentFlags);

        void Swap();

        int GetScreenWidth() { return m_screenWidth; }
        int GetScreenHeight() { return m_screenHeight; }

    private:
        SDL_Window* m_sdlWindow;
        int m_screenWidth, m_screenHeight;
    };
}
