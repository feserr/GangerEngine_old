#pragma once

#include <GangerEngine/IMainGame.h>
#include "GameplayScreen.h"

// Our custom app that inherits from IMainGame
class App : public GangerEngine::IMainGame{
public:
    App();
    ~App();

    // Called on initialization
    virtual void OnInit() override;
    // For adding all screens
    virtual void AddScreens() override;
    // Called when exiting
    virtual void OnExit() override;
private:
    std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
};

