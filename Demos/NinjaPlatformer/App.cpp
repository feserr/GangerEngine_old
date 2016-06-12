#include "App.h"

#include <GangerEngine/ScreenList.h>

App::App() {

}

App::~App() {

}

void App::OnInit() {

}

void App::AddScreens() {
    m_gameplayScreen = std::make_unique<GameplayScreen>();

    m_screenList->AddScreen(m_gameplayScreen.get());
    m_screenList->SetScreen(m_gameplayScreen->GetScreenIndex());
}

void App::OnExit() {

}
