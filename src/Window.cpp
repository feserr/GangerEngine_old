
#include <GangerEngine/Window.h>
#include <GangerEngine/Errors.h>

namespace GangerEngine
{
    Window::Window()
    {
    }

    Window::~Window()
    {
    }

    int Window::Create(std::string windowName, int screenWidth, int screenHeight,
        unsigned int currentFlags)
    {
        Uint32 flags = SDL_WINDOW_OPENGL;

        flags |= (currentFlags & INVISIBLE ? SDL_WINDOW_HIDDEN : 0x0);
        flags |= (currentFlags & FULLSCREEN ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0x0);
        flags |= (currentFlags & BORDERLESS ? SDL_WINDOW_BORDERLESS : 0x0);

        _sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
        if(_sdlWindow == nullptr)
        {
            FatalError("SDL window: Could not be created");
        }

        // Set up our OpenGL context
        SDL_GLContext glContext = SDL_GL_CreateContext(_sdlWindow);
        if(glContext == nullptr)
        {
            FatalError("SDL_GLContext: Could not be created");
        }

        // Set up GLEW (optional but recommended)
        GLenum error = glewInit();
        if(error != GLEW_OK)
        {
            FatalError("GLEW: Could not be initialized");
        }

        // Check the OpenGL version
        printf("***  OpenGL version %s  ***\n", glGetString(GL_VERSION));

        // Set up the background color
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

        // Set VSYNC
        SDL_GL_SetSwapInterval(1);

        return 1;
    }

    void Window::Swap()
    {
        // Swap our buffer and draw everything to the screen
        SDL_GL_SwapWindow(_sdlWindow);
    }
}