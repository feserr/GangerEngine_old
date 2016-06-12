// ScreenList.h

#pragma once

#include <vector>

namespace GangerEngine {

    class IMainGame;
    class IGameScreen;

    class ScreenList {
    public:
        ScreenList(IMainGame* game);
        ~ScreenList();

        IGameScreen* MoveNext();
        IGameScreen* MovePrevious();

        void SetScreen(int nextScreen);
        void AddScreen(IGameScreen* newScreen);

        void Destroy();

        IGameScreen* GetCurrent();

    protected:
        IMainGame* m_game = nullptr;
        std::vector<IGameScreen*> m_screens;
        int m_currentScreenIndex = -1;
    };

}