/*
    Copyright [2016] [Ganger Games]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include <GangerEngine/IMainGame.h>
#include <GangerEngine/Timing.h>
#include <GangerEngine/ScreenList.h>
#include <GangerEngine/IGameScreen.h>

namespace GangerEngine {
    IMainGame::IMainGame() {
        m_screenList = std::make_unique<ScreenList>(this);
    }

    IMainGame::~IMainGame() {
        // Empty
    }

    void IMainGame::Run() {
        if (!Init()) return;

        FpsLimiter limiter;
        limiter.SetMaxFPS(60.0f);

        // Game loop
        m_isRunning = true;
        while (m_isRunning) {
            limiter.Begin();

            inputManager.Update();
            // Call the custom update and draw method
            Update();
            if (m_isRunning) {
                Draw();

                m_fps = limiter.End();
                m_window.SwapBuffer();
            }
        }
    }

    void IMainGame::ExitGame() {
        m_currentScreen->OnExit();
        if (m_screenList) {
            m_screenList->Destroy();
            m_screenList.reset();
        }
        m_isRunning = false;
    }

    void IMainGame::OnSDLEvent(const SDL_Event& evnt) {
        switch (evnt.type) {
            case SDL_QUIT:
                ExitGame();
                break;
            case SDL_MOUSEMOTION:
                inputManager.SetMouseCoords(
                    static_cast<float>(evnt.motion.x),
                    static_cast<float>(evnt.motion.y));
                break;
            case SDL_KEYDOWN:
                inputManager.PressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                inputManager.ReleaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                inputManager.PressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                inputManager.ReleaseKey(evnt.button.button);
                break;
        }
    }

    bool IMainGame::Init() {
        GangerEngine::Init();

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

        if (!InitSystems()) return false;

        OnInit();
        AddScreens();

        m_currentScreen = m_screenList->GetCurrent();
        m_currentScreen->OnEntry();
        m_currentScreen->SetRunning();

        return true;
    }

    bool IMainGame::InitSystems() {
        m_window.Create("Default", 1280, 720, 0);
        return true;
    }


    void IMainGame::Update() {
        if (m_currentScreen) {
            switch (m_currentScreen->GetState()) {
                case ScreenState::RUNNING:
                    m_currentScreen->Update();
                    break;
                case ScreenState::CHANGE_NEXT:
                    m_currentScreen->OnExit();
                    m_currentScreen = m_screenList->MoveNext();
                    if (m_currentScreen) {
                        m_currentScreen->SetRunning();
                        m_currentScreen->OnEntry();
                    }
                    break;
                case ScreenState::CHANGE_PREVIOUS:
                    m_currentScreen->OnExit();
                    m_currentScreen = m_screenList->MovePrevious();
                    if (m_currentScreen) {
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
        } else {
            ExitGame();
        }
    }

    void IMainGame::Draw() {
        glViewport(0, 0, m_window.GetScreenWidth(), m_window.GetScreenHeight());
        if (m_currentScreen && m_currentScreen->GetState() ==
            ScreenState::RUNNING) {
            m_currentScreen->Draw();
        }
    }
}  // namespace GangerEngine
