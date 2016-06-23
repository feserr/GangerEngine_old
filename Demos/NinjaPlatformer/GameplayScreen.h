#pragma once

#include "Box.h"
#include "Player.h"
#include <GangerEngine/IGameScreen.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <GangerEngine/SpriteBatch.h>
#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/Camera2D.h>
#include <GangerEngine/GLTexture.h>
#include <GangerEngine/Window.h>
#include <GangerEngine/DebugRenderer.h>

#include <GangerEngine/GUI.h>

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
    GangerEngine::GLSLProgram m_lightProgram;
    GangerEngine::Camera2D m_camera;
    GangerEngine::GLTexture m_texture;
    GangerEngine::Window* m_window;
    GangerEngine::DebugRenderer m_debugRenderer;
    GangerEngine::GUI m_gui;

    bool m_renderDebug = false;

    Player m_player;
    std::vector<Box> m_boxes;
    std::unique_ptr<b2World> m_world;
};

