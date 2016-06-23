#pragma once

#include "Player.h"
#include <GangerEngine/IGameScreen.h>
#include <vector>
#include <GangerEngine/SpriteBatch.h>
#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/Camera2D.h>
#include <GangerEngine/GLTexture.h>
#include <GangerEngine/Window.h>
#include <GangerEngine/DebugRenderer.h>

#include <GangerEngine/GUI.h>

#include "ScreenIndices.h"

class MainMenuScreen : public GangerEngine::IGameScreen {
public:
    MainMenuScreen(GangerEngine::Window* window);
    ~MainMenuScreen();

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

    bool onNewGameClicked(const CEGUI::EventArgs& e);
    bool onEditorClicked(const CEGUI::EventArgs& e);
    bool onExitClicked(const CEGUI::EventArgs& e);

    int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
    GangerEngine::Camera2D m_camera;
    GangerEngine::Window* m_window;
    GangerEngine::GUI m_gui;
};