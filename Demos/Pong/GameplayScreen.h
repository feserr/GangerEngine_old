/*
    Copyright [2016] [Elías Serrano]

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

#ifndef GameplayScreen_h
#define GameplayScreen_h

#include "Ball.h"
#include "Player.h"
#include "Environment.h"

#include <GangerEngine/IGameScreen.h>
#include <GangerEngine/SpriteBatch.h>
#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/Camera2D.h>
#include <GangerEngine/GLTexture.h>
#include <GangerEngine/Window.h>
#include <GangerEngine/DebugRenderer.h>
#include <GangerEngine/GUI.h>
#include <vector>

// Our custom gameplay screen that inherits from the IGameScreen
class GameplayScreen : public GangerEngine::IGameScreen {
public:
    GameplayScreen(GangerEngine::Window* window);
    ~GameplayScreen();

    virtual int GetNextScreenIndex() const override;

    virtual int GetPreviousScreenIndex() const override;

    virtual void Build() override;

    virtual void Destroy() override;

    virtual void OnEntry() override;

    virtual void OnExit() override;

    virtual void Update() override;

    virtual void Draw() override;

private:
    void initUI();
    void checkInput();

    bool onExitClicked(const CEGUI::EventArgs& e);

    GangerEngine::SpriteBatch m_spriteBatch;
    GangerEngine::GLSLProgram m_textureProgram;
    GangerEngine::Camera2D m_camera;
    GangerEngine::GLTexture m_texture;
    GangerEngine::Window* m_window;
    GangerEngine::DebugRenderer m_debugRenderer;
    GangerEngine::GUI m_gui;

    bool m_renderDebug = false;

    Player m_playerOne;
    Player m_playerTwo;
    Ball m_ball;
    std::vector<Environment> m_enviroment;

    int m_iScorePlayerOne = 0, m_iScorePlayerTwo = 0;

    CEGUI::DefaultWindow *m_scorePlayerOne;
    CEGUI::DefaultWindow *m_scorePlayerTwo;

    glm::vec2 m_windowSize;
};

#endif /* GameplayScreen_h */
