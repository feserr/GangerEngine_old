// IMainGame.h

#pragma once

#include <memory>

#include "GangerEngine.h"
#include "Window.h"
#include "InputManager.h"

namespace GangerEngine
{
    class ScreenList;
    class IGameScreen;

    class IMainGame {
    public:
        IMainGame();
        virtual ~IMainGame();

        // Runs and initializes the game
        void Run();
        // Exits the game
        void ExitGame();

        // Called on initialization
        virtual void OnInit() = 0;
        // For adding all screens
        virtual void AddScreens() = 0;
        // Called when exiting
        virtual void OnExit() = 0;

        void OnSDLEvent(SDL_Event& evnt);

        const float GetFps() const { return m_fps; }

    protected:
        // Custom update function
        virtual void Update();
        // Custom render function
        virtual void Draw();

        bool Init();
        bool InitSystems();

        std::unique_ptr<ScreenList> m_screenList;
        IGameScreen* m_currentScreen = nullptr;
        bool m_isRunning = false;
        float m_fps = 0.0f;
        Window m_window;
        InputManager m_inputManager;
    };

}