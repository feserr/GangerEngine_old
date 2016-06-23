#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <GangerEngine/IMainGame.h>
#include <GangerEngine/ResourceManager.h>
#include <random>

#include "ScreenIndices.h"

#include "Light.h"

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

    b2Vec2 gravity(0.0f, -25.0);
    m_world = std::make_unique<b2World>(gravity);

    m_debugRenderer.Init();

    // Load the texture
    m_texture = GangerEngine::ResourceManager::GetTexture("Assets/bricks_top.png");

    // Make the ground
    Box groundBox;
    groundBox.init(m_world.get(), glm::vec2(0.0f, -20.0f), glm::vec2(50.0f, 10.0f), m_texture, GangerEngine::ColorRGBA8(255, 255, 255, 255), false, false);
    m_boxes.push_back(groundBox);

    // Make a bunch of boxes
    std::mt19937 randGenerator;
    std::uniform_real_distribution<float> xPos(-10.0, 10.0f);
    std::uniform_real_distribution<float> yPos(-10.0, 25.0f);
    std::uniform_real_distribution<float> size(0.5, 2.5f);
    std::uniform_int_distribution<int> color(50, 255);
    const int NUM_BOXES = 10;

    for (int i = 0; i < NUM_BOXES; i++) {
        GangerEngine::ColorRGBA8 randColor;
        randColor.r = color(randGenerator);
        randColor.g = color(randGenerator);
        randColor.b = color(randGenerator);
        randColor.a = 255;
        Box newBox;
        newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)), m_texture, randColor, false, true);
        m_boxes.push_back(newBox);
    }

    // Initialize spritebatch
    m_spriteBatch.Init();

    // Shader init
    // Compile our texture
    m_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.AddAttribute("vertexPosition");
    m_textureProgram.AddAttribute("vertexUV");
    m_textureProgram.AddAttribute("vertexColor");
    m_textureProgram.LinkShaders();
    // Compile our light shader
    m_lightProgram.CompileShaders("Shaders/lightShading.vert", "Shaders/lightShading.frag");
    m_lightProgram.AddAttribute("vertexPosition");
    m_lightProgram.AddAttribute("vertexUV");
    m_lightProgram.AddAttribute("vertexColor");
    m_lightProgram.LinkShaders();

    // Init camera
    m_camera.Init(m_window->GetScreenWidth(), m_window->GetScreenHeight());
    m_camera.SetScale(16.0f);

    // Init player
    glm::vec2 position(0.0f, 30.0f);
    glm::vec2 drawDims(2.0f);
    glm::vec2 collisionDims(1.0f, 1.8f);
    m_player.init(m_world.get(), position, drawDims, collisionDims,
        GangerEngine::ColorRGBA8(255, 255, 255, 255));

    initUI();
}

void GameplayScreen::OnExit() {
    m_debugRenderer.Dispose();
    m_boxes.clear();
    m_world.reset();
}

void GameplayScreen::Update() {
    m_camera.Update();
    checkInput();
    m_player.update(m_game->inputManager);

    // Update the physics simulation
    m_world->Step(1.0f / 60.0f, 6, 2);
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

    // Draw all the boxes
    for (auto& b : m_boxes) {
        b.draw(m_spriteBatch);
    }
    m_player.draw(m_spriteBatch);

    m_spriteBatch.End();
    m_spriteBatch.RenderBatch();
    m_textureProgram.Unuse();

    // Debug rendering
    if(m_renderDebug) {
        glm::vec4 destRect;
        for (auto& b : m_boxes) {       
            destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
            destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
            destRect.z = b.getDimensions().x;
            destRect.w = b.getDimensions().y;
            m_debugRenderer.DrawBox(destRect, GangerEngine::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());
        }
        m_player.drawDebug(m_debugRenderer);
        // Render player
        m_debugRenderer.End();
        m_debugRenderer.Render(projectionMatrix, 2.0f);
    }

    // Render some test lights
    // TODO: Don't hardcode this!
    Light playerLight;
    playerLight.color = GangerEngine::ColorRGBA8(255, 255, 255, 128);
    playerLight.position = m_player.getPosition();
    playerLight.size = 30.0f;

    Light mouseLight;
    mouseLight.color = GangerEngine::ColorRGBA8(255, 0, 255, 150);
    mouseLight.position = m_camera.ConvertScreenToWorld(m_game->inputManager.GetMouseCoords());
    mouseLight.size = 45.0f;

    m_lightProgram.Use();
    pUniform = m_textureProgram.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    m_spriteBatch.Begin();

    playerLight.draw(m_spriteBatch);
    mouseLight.draw(m_spriteBatch);

    m_spriteBatch.End();
    m_spriteBatch.RenderBatch();

    m_lightProgram.Unuse();

    // Reset to regular alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_gui.Draw();
}

void GameplayScreen::initUI() {
    // Init the UI
    m_gui.Init("GUI");
    m_gui.LoadScheme("TaharezLook.scheme");
    m_gui.SetFont("DejaVuSans-10");
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.CreateWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
    testButton->setText("Exit Game!");

    // Set the event to be called when we click
    testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

    m_gui.SetMouseCursor("TaharezLook/MouseArrow");
    m_gui.ShowMouseCursor();
    SDL_ShowCursor(0);
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