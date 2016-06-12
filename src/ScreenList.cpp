// ScreenList.cpp

#include "ScreenList.h"

#include "IGameScreen.h"

namespace GangerEngine
{
    ScreenList::ScreenList(IMainGame* game) :
        m_game(game)
    {
        // Empty
    }

    ScreenList::~ScreenList()
    {
        Destroy();
    }

    IGameScreen* ScreenList::MoveNext()
    {
        IGameScreen* currentScreen = GetCurrent();

        if(currentScreen->GetNextScreenIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            m_currentScreenIndex = currentScreen->GetNextScreenIndex();
        }

        return GetCurrent();
    }

    IGameScreen* ScreenList::MovePrevious()
    {
        IGameScreen* currentScreen = GetCurrent();

        if(currentScreen->GetPreviousScreenIndex() != SCREEN_INDEX_NO_SCREEN)
        {
            m_currentScreenIndex = currentScreen->GetPreviousScreenIndex();
        }

        return GetCurrent();
    }

    void ScreenList::SetScreen(int nextScreen)
    {
        m_currentScreenIndex = nextScreen;
    }

    void ScreenList::AddScreen(IGameScreen* newScreen)
    {
        newScreen->m_screenIndex = m_screens.size();
        m_screens.push_back(newScreen);
        newScreen->Build();
        newScreen->SetParentGame(m_game);
    }

    void ScreenList::Destroy()
    {
        for(size_t i = 0; i < m_screens.size(); i++)
        {
            m_screens[i]->Destroy();
        }

        m_screens.resize(0);
        m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
    }

    IGameScreen* ScreenList::GetCurrent()
    {
        if(m_currentScreenIndex == SCREEN_INDEX_NO_SCREEN)
            return nullptr;

        return m_screens[m_currentScreenIndex];
    }

}