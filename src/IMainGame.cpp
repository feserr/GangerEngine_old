// IMainGame.cpp

#include "IMainGame.h"

#include "Timing.h"
#include "ScreenList.h"
#include "IGameScreen.h"

namespace GangerEngine
{
    IMainGame::IMainGame()
    {
        m_screenList = std::make_unique<ScreenList>(this);
    }

    IMainGame::~IMainGame()
    {
        // Empty
    }

    void IMainGame::Run()
    {
        if (!Init())
            return;

        FpsLimiter limiter;
        limiter.SetTargetFPS(60.0f);

        // Game loop
        m_isRunning = true;
        while(m_isRunning)
        {
            limiter.Begin();

            // Call the custom update and draw method
            m_inputManager.Update();
            Update();
            Draw();

            m_fps = limiter.End();
            m_window.Swap();
        }

    }

    void IMainGame::ExitGame()
    {
        m_currentScreen->OnExit();

        if(m_screenList)
        {
            m_screenList->Destroy();
            m_screenList.reset();
        }

        m_isRunning = false;
    }

    void IMainGame::OnSDLEvent(SDL_Event& evnt)
    {
        switch(evnt.type)
        {
            case SDL_QUIT:
                m_isRunning = false;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.SetMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.PressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.ReleaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.PressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.ReleaseKey(evnt.button.button);
                break;
        }
    }

    bool IMainGame::Init()
    {
        GangerEngine::Init();

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        if(!InitSystems())
            return false;

        OnInit();
        AddScreens();

        m_currentScreen = m_screenList->GetCurrent();
        m_currentScreen->OnEntry();
        m_currentScreen->SetRunning();

        return true;
    }

    bool IMainGame::InitSystems()
    {
        m_window.Create("Default", 1280, 720, 0);
        return true;
    }


    void IMainGame::Update()
    {
        if(m_currentScreen)
        {
            switch(m_currentScreen->GetState()) {
                case ScreenState::RUNNING:
                    m_currentScreen->Update();

                    break;
                case ScreenState::CHANGE_NEXT:
                    m_currentScreen->OnExit();
                    m_currentScreen = m_screenList->MoveNext();

                    if(m_currentScreen)
                    {
                        m_currentScreen->SetRunning();
                        m_currentScreen->OnEntry();
                    }

                    break;
                case ScreenState::CHANGE_PREVIOUS:
                    m_currentScreen->OnExit();
                    m_currentScreen = m_screenList->MovePrevious();

                    if(m_currentScreen)
                    {
                        m_currentScreen->SetRunning();
                        m_currentScreen->OnEntry();
                    }

                    break;
                case ScreenState::EXIT_APPLICATION:
                    ExitGame();

                    break;
                default:
                    break;
            }
        }
        else
        {
            ExitGame();
        }
    }

    void IMainGame::Draw()
    {
        glViewport(0, 0, m_window.GetScreenWidth(), m_window.GetScreenHeight());
        if (m_currentScreen && m_currentScreen->GetState() == ScreenState::RUNNING) {
            m_currentScreen->Draw();
        }
    }
}