#include "App.h"
#include <GangerEngine/ScreenList.h>


App::App() {

}

App::~App() {

}

void App::OnInit() {

}

void App::AddScreens() {
    m_mainMenuScreen = std::make_unique<MainMenuScreen>(&m_window);
    m_gameplayScreen = std::make_unique<GameplayScreen>(&m_window);
    m_editorScreen = std::make_unique<EditorScreen>(&m_window);

    m_screenList->AddScreen(m_mainMenuScreen.get());
    m_screenList->AddScreen(m_gameplayScreen.get());
    m_screenList->AddScreen(m_editorScreen.get());

    m_screenList->SetScreen(m_mainMenuScreen->GetScreenIndex());
}

void App::OnExit() {

}
