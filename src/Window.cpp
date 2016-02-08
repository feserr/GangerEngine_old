
#include <GangerEngine/Window.h>
#include <GangerEngine/GangerErrors.h>

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

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);



        m_sdlWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);
        if(m_sdlWindow == nullptr)
        {
            FatalError("SDL window: Could not be created");
        }

        // Set up our OpenGL context
        SDL_GLContext glContext = SDL_GL_CreateContext(m_sdlWindow);
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

        // Init glew for OSX systems
#if __APPLE__
            glewExperimental = GL_TRUE;
            glewInit();
#endif

        // Check the OpenGL version
        printf("***  OpenGL version %s  ***\n", glGetString(GL_VERSION));
        printf("***  Supported GLSL version is %s  ***\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

        // Set up the background color
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

        // Set VSYNC
        SDL_GL_SetSwapInterval(1);

        // Enable alpha blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return 1;
    }

    void Window::Swap()
    {
        // Swap our buffer and draw everything to the screen
        SDL_GL_SwapWindow(m_sdlWindow);
    }
}