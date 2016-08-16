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

#include "GameplayScreen.h"
#include "ScreenIndices.h"

#include <SDL/SDL.h>
#include <GangerEngine/IMainGame.h>
#include <GangerEngine/ResourceManager.h>
#include <iostream>
#include <random>

const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

GameplayScreen::GameplayScreen(GangerEngine::Window* window) : m_window(window){
    m_screenIndex = SCREEN_INDEX_GAMEPLAY;
}

GameplayScreen::~GameplayScreen() {

}

int GameplayScreen::GetNextScreenIndex() const {
    return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::GetPreviousScreenIndex() const {
    return SCREEN_INDEX_MAINMENU;
}

void GameplayScreen::Build() {
}

void GameplayScreen::Destroy() {
}

void GameplayScreen::OnEntry() {
    m_debugRenderer.Init();

    // Initialize spritebatch
    m_spriteBatch.Init();

    // Shader init
    // Compile our texture
    m_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.AddAttribute("vertexPosition");
    m_textureProgram.AddAttribute("vertexUV");
    m_textureProgram.AddAttribute("vertexColor");
    m_textureProgram.LinkShaders();

    // Init camera
    m_camera.Init(m_window->GetScreenWidth(), m_window->GetScreenHeight());
    m_camera.SetPosition(glm::vec2(m_window->GetScreenWidth() / 2.0f,
        m_window->GetScreenHeight() / 2.0f));
    m_camera.SetScale(1.0f);

    // Init players
    glm::vec2 position(32.0f, m_window->GetScreenHeight() / 2);
    glm::vec2 drawDims(32.0f, 32.0f);
    glm::vec2 collisionDims(32.0f, 48.0f);
    m_playerOne.Init(position, drawDims, collisionDims,
        GangerEngine::ColorRGBA8(255, 255, 255, 255), PlayerType::PLAYERONE);

    position = glm::vec2(m_window->GetScreenWidth() - 32.0f,
        m_window->GetScreenHeight() / 2);
    m_playerTwo.Init(position, drawDims, collisionDims,
        GangerEngine::ColorRGBA8(255, 255, 255, 255), PlayerType::PLAYERTWO);

    // Init the ball
    position = glm::vec2(m_window->GetScreenWidth() / 2,
        m_window->GetScreenHeight() / 2);
    glm::vec2 vel(-6.0f, 0.0f);
    m_ball.Init(8.0f, 0.0f, position, vel);

    // Init the environment
    position = glm::vec2(m_window->GetScreenWidth() / 2, 0.0f);
    for(int i = 16; i <= m_window->GetScreenHeight(); i += 64) {
        Environment environment;
        position.y = i;
        environment.Init(position, drawDims,
            GangerEngine::ColorRGBA8(255, 255, 255, 130));
        m_enviroment.push_back(environment);
    }

    m_windowSize = glm::vec2(m_window->GetScreenWidth(),
        m_window->GetScreenHeight());

    initUI();
}

void GameplayScreen::OnExit() {
    m_debugRenderer.Dispose();
}

void GameplayScreen::Update() {
    checkInput();

    // Calculate the frameTime in milliseconds
    static Uint32 previousTicks = SDL_GetTicks();
    Uint32 newTicks = SDL_GetTicks();
    Uint32 frameTime = newTicks - previousTicks;
    previousTicks = newTicks;
    // Get the total delta time
    float totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;

    int i = 0;
    int goal = 0;
    while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
        // The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
        float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
        // Update all physics here and pass in deltaTime

        int t_goal = m_ball.Update(deltaTime, m_windowSize);
        m_playerOne.Update(deltaTime, m_game->inputManager, m_windowSize, m_ball);
        m_playerTwo.Update(deltaTime, m_game->inputManager, m_windowSize, m_ball);

        // Since we just took a step that is length deltaTime, subtract from
        // totalDeltaTime
        totalDeltaTime -= deltaTime;
        ++i;

        // Prevent scoring more than one goal
        if (goal == 0) {
            goal = t_goal;
        }
    }

    if (goal == 1) {
        ++m_iScorePlayerTwo;
        m_scorePlayerTwo->setText(std::to_string(m_iScorePlayerTwo));
    } else if (goal == 2) {
        ++m_iScorePlayerOne;
        m_scorePlayerOne->setText(std::to_string(m_iScorePlayerOne));
    }

    m_camera.Update();
}

void GameplayScreen::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_textureProgram.Use();

    // Upload texture uniform
    GLint textureUniform = m_textureProgram.GetUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    // Camera matrix
    glm::mat4 projectionMatrix = m_camera.GetCameraMatrix();
    GLint pUniform = m_textureProgram.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_spriteBatch.Begin();

    m_playerOne.Draw(m_spriteBatch);
    m_playerTwo.Draw(m_spriteBatch);
    m_ball.Draw(m_spriteBatch);
    for(Environment environment : m_enviroment) {
        environment.Draw(m_spriteBatch);
    }

    m_spriteBatch.End();
    m_spriteBatch.RenderBatch();
    m_textureProgram.Unuse();

    // Debug rendering
    if(m_renderDebug) {
        m_playerOne.DrawDebug(m_debugRenderer);
        m_playerTwo.DrawDebug(m_debugRenderer);
        m_ball.DrawDebug(m_debugRenderer);
        // Render player
        m_debugRenderer.End();
        m_debugRenderer.Render(projectionMatrix, 2.0f);
    }

    pUniform = m_textureProgram.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Reset to regular alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_gui.Draw();
}

void GameplayScreen::initUI() {
    // Init the UI
    m_gui.Init("Assets/Pong/GUI");
    m_gui.LoadScheme("VanillaSkin.scheme");
    m_gui.SetFont("nokiafc22-10");
    m_gui.SetSize(m_window->GetScreenWidth(), m_window->GetScreenHeight());
    CEGUI::PushButton* exitButton =
        static_cast<CEGUI::PushButton*>(m_gui.CreateWidget("Vanilla/Button",
            glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f),
            "ExitButton"));
    exitButton->setText("Exit");

    // Set the event to be called when we click
    exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
        CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

    m_scorePlayerOne =
        static_cast<CEGUI::DefaultWindow*>(m_gui.CreateWidget(
        "Vanilla/Label", glm::vec4(0.25f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f),
        "ScorePlayerOne"));
    m_scorePlayerOne->setText("0");
    m_scorePlayerOne->setProperty("HorzFormatting", "HorzCentred");

    m_scorePlayerTwo =
    static_cast<CEGUI::DefaultWindow*>(m_gui.CreateWidget(
        "Vanilla/Label", glm::vec4(0.75f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f),
        "ScorePlayerTwo"));
    m_scorePlayerTwo->setText("0");
    m_scorePlayerTwo->setProperty("HorzFormatting", "HorzCentred");

    //m_gui.SetMouseCursor("TaharezLook/MouseArrow");
    //m_gui.ShowMouseCursor();
    SDL_ShowCursor(1);
}

void GameplayScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
        m_game->OnSDLEvent(evnt);
        m_gui.OnSDLEvent(evnt);
        switch (evnt.type) {
            case SDL_QUIT:
                onExitClicked(CEGUI::EventArgs());
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                m_gui.SetSize(m_window->GetScreenWidth(), m_window->
                    GetScreenHeight());
                break;
        }
    }

    if (m_game->inputManager.IsKeyPressed(SDLK_0)) {
        m_renderDebug = !m_renderDebug;
    }
}

bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = GangerEngine::ScreenState::EXIT_APPLICATION;
    return true;
}
