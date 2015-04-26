
#include "MainGame.h"
#include "Errors.h"

#include <stdio.h>
#include <string>

MainGame::MainGame() : _window(nullptr), _screenWidth(1024), _screenHeight(768), 
    _gameState(GameState::PLAY), _time(0)
{
}


MainGame::~MainGame()
{
}

void MainGame::Run()
{
    InitSystems();

    _sprite.Init(-1.f, -1.f, 2.f, 2.f);

    GameLoop();
}

void MainGame::InitSystems()
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        std::printf("SDL could not be initialised");
        exit(1);
    }

    _window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
    if (_window == nullptr)
    {
        FatalError("SDL window: Could not be created");
    }

    // set up our opengl context
    SDL_GLContext glContext = SDL_GL_CreateContext(_window);
    if (glContext == nullptr)
    {
        FatalError("SDL_GLContext: Could not be created");
    }

    // set up glew (optional but recommended)
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        FatalError("GLEW: Could not be initialized");
    }

    // use double buffer to avoid flickring
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // set up the background color
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

    InitShaders();
}

void MainGame::InitShaders()
{
    _colorProgram.CompileShaders(std::string("Shaders/ColorShading.vert"),
        std::string("Shaders/ColorShading.frag"));
    _colorProgram.AddAttribute("vertexPosition");
    _colorProgram.AddAttribute("vertexColor");
    _colorProgram.LinkShaders();
}

void MainGame::GameLoop()
{
    while (_gameState != GameState::EXIT)
    {
        ProcessInput();
        _time += 0.001f;
        DrawGame();
    }
}

void MainGame::ProcessInput()
{
    SDL_Event ev;

    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
            case SDL_QUIT:
            {
                _gameState = GameState::EXIT;
                break;
            }
            case SDL_MOUSEMOTION:
            {
                printf("Mouse axis %dx%d\n", ev.motion.x, ev.motion.y);
                break;
            }
            default:
                break;
        }
    }
}

void MainGame::DrawGame()
{
    // Set the base depth
    glClearDepth(1.f);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _colorProgram.use();

    GLuint timeLocation = _colorProgram.getUniformLocation("time");
    glUniform1f(timeLocation, _time);

    _sprite.Draw();

    _colorProgram.unuse();

    // Swap our buffer and draw everything to the screen
    SDL_GL_SwapWindow(_window);
}
