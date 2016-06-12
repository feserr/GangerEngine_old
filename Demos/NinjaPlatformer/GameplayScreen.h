#pragma once

#include <GangerEngine/IGameScreen.h>

// Our custom gameplay screen that inherits from the IGameScreen
class GameplayScreen : public GangerEngine::IGameScreen {
public:
    GameplayScreen();
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
    void checkInput();

};

