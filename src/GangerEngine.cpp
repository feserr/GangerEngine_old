
#include <GangerEngine/GangerEngine.h>

#include <SDL/SDL.h>
#include <GL/glew.h>

#include <stdio.h>

namespace GangerEngine
{
    int Init()
    {
        if(SDL_Init(SDL_INIT_EVERYTHING))
        {
            printf("SDL could not be initialized");
            exit(1);
        }

        // Use double buffer to avoid flickering
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        return 0;
    }
}
