#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen(GangerEngine::Window* window) : m_window(window) {
    m_screenIndex = SCREEN_INDEX_MAINMENU;
}

MainMenuScreen::~MainMenuScreen() {
}

int MainMenuScreen::GetNextScreenIndex() const {
    return m_nextScreenIndex;
}

int MainMenuScreen::GetPreviousScreenIndex() const {
    return SCREEN_INDEX_NO_SCREEN;
}

void MainMenuScreen::Build() {

}

void MainMenuScreen::Destroy() {

}

void MainMenuScreen::OnEntry() {
    // Init camera
    m_camera.Init(m_window->GetScreenWidth(), m_window->GetScreenHeight());
    m_camera.SetScale(32.0f);

    initUI();
}

void MainMenuScreen::OnExit() {
    m_gui.Destroy();
}

void MainMenuScreen::Update() {
    m_camera.Update();
    checkInput();
}

void MainMenuScreen::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

    m_gui.Draw();
}

void MainMenuScreen::initUI() {
    // Init the UI
    m_gui.Init("GUI");
    m_gui.LoadScheme("TaharezLook.scheme");
    m_gui.SetFont("DejaVuSans-10");

    CEGUI::PushButton* playGameButton = static_cast<CEGUI::PushButton*>(m_gui.CreateWidget("TaharezLook/Button", glm::vec4(0.45f, 0.5f, 0.1f, 0.05f), glm::vec4(0.0f), "NewGameButton"));
    playGameButton->setText("New Game");
    // Set up event to be called when we click
    playGameButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onNewGameClicked, this));

    CEGUI::PushButton* editorButton = static_cast<CEGUI::PushButton*>(m_gui.CreateWidget("TaharezLook/Button", glm::vec4(0.45f, 0.56f, 0.1f, 0.05f), glm::vec4(0.0f), "EditorButton"));
    editorButton->setText("Level Editor");
    // Set up event to be called when we click
    editorButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onEditorClicked, this));

    CEGUI::PushButton* exitButton = static_cast<CEGUI::PushButton*>(m_gui.CreateWidget("TaharezLook/Button", glm::vec4(0.45f, 0.62f, 0.1f, 0.05f), glm::vec4(0.0f), "ExitButton"));
    exitButton->setText("Exit Game");
    // Set the event to be called when we click
    exitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&MainMenuScreen::onExitClicked, this));

    m_gui.SetMouseCursor("TaharezLook/MouseArrow");
    m_gui.ShowMouseCursor();
    SDL_ShowCursor(0);
}

void MainMenuScreen::checkInput() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
        m_gui.OnSDLEvent(evnt);
        switch (evnt.type) {
            case SDL_QUIT:
                onExitClicked(CEGUI::EventArgs());
                break;
        }
    }
}

bool MainMenuScreen::onNewGameClicked(const CEGUI::EventArgs& e) {
    m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
    m_currentState = GangerEngine::ScreenState::CHANGE_NEXT;
    return true;
}

bool MainMenuScreen::onEditorClicked(const CEGUI::EventArgs& e) {
    m_nextScreenIndex = SCREEN_INDEX_EDITOR;
    m_currentState = GangerEngine::ScreenState::CHANGE_NEXT;
    return true;
}

bool MainMenuScreen::onExitClicked(const CEGUI::EventArgs& e) {
    m_currentState = GangerEngine::ScreenState::EXIT_APPLICATION;
    return true;
}
