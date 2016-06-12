#include "GameplayScreen.h"
#include <iostream>
#include <SDL/SDL.h>
#include <GangerEngine/IMainGame.h>

GameplayScreen::GameplayScreen() {
}

GameplayScreen::~GameplayScreen() {

}

int GameplayScreen::GetNextScreenIndex() const {
    return GangerEngine::SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::GetPreviousScreenIndex() const {
    return GangerEngine::SCREEN_INDEX_NO_SCREEN;
}

void GameplayScreen::Build() {
}

void GameplayScreen::Destroy() {
}

void GameplayScreen::OnEntry() {

}

void GameplayScreen::OnExit() {
}

void GameplayScreen::Update() {
    checkInput();
}

void GameplayScreen::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
}

void GameplayScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
        m_game->OnSDLEvent(evnt);
    }
}
