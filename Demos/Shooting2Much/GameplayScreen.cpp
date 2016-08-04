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

    // Load the texture
    //m_texture = GangerEngine::ResourceManager::GetTexture("Assets/bricks_top.png");

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
    m_camera.SetScale(32.0f);

    // Init player
    glm::vec2 position(0.0f, 0.0f);
    glm::vec2 drawDims(1.0f, 1.0f);
    glm::vec2 collisionDims(1.0f, 1.8f);
    m_player.Init(position, drawDims, collisionDims,
        GangerEngine::ColorRGBA8(255, 255, 255, 255));

    initUI();
}

void GameplayScreen::OnExit() {
    m_debugRenderer.Dispose();
}

void GameplayScreen::Update() {
    m_camera.Update();
    checkInput();
    m_player.Update(m_game->inputManager);
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

    m_player.Draw(m_spriteBatch);

    m_spriteBatch.End();
    m_spriteBatch.RenderBatch();
    m_textureProgram.Unuse();

    // Debug rendering
    if(m_renderDebug) {
        m_player.DrawDebug(m_debugRenderer);
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
    m_gui.Init("GUI");
    m_gui.LoadScheme("VanillaSkin.scheme");
    m_gui.SetFont("DejaVuSans-10");
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.CreateWidget("Vanilla/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
    testButton->setText("Exit Game!");

    // Set the event to be called when we click
    testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

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
        }
    }
}

bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = GangerEngine::ScreenState::EXIT_APPLICATION;
    return true;
}
