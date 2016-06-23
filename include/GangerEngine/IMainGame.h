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

#ifndef _IMAINGAME_H_
#define _IMAINGAME_H_

#include <GangerEngine/GangerEngine.h>
#include <GangerEngine/Window.h>
#include <GangerEngine/InputManager.h>

#include <memory>

namespace GangerEngine {
class ScreenList;
class IGameScreen;

class IMainGame {
 public:
    IMainGame();
    virtual ~IMainGame();

    /// Runs and initializes the game
    void Run();
    /// Exits the game
    void ExitGame();

    /// Called on initialization
    virtual void OnInit() = 0;
    /// For adding all screens
    virtual void AddScreens() = 0;
    /// Called when exiting
    virtual void OnExit() = 0;

    void OnSDLEvent(const SDL_Event& evnt);

    const float GetFps() const { return m_fps; }

    InputManager inputManager;

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
};
}  // namespace GangerEngine

#endif  // _IMAINGAME_H_
