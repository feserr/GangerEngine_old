#include "Errors.h"

#include <SDL\SDL.h>
#include <iostream>
#include <cstdlib>

void FatalError(std::string errorString)
{
    do
    {
        std::cout << '\n' << errorString <<"\n\nPress a key to continue...";
    } while (std::cin.get() != '\n');

    SDL_Quit();
    exit(70);
}
