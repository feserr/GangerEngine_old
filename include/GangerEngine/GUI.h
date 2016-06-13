// GUI.h

#pragma once

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>
#include <SDL/SDL_events.h>

namespace GangerEngine
{
    class GUI
    {
    public:
        void Init(const std::string& resourceDirectory);
        void Destroy();

        void Draw();
        void Update();

        void SetMouseCursor(const std::string& imageFile);
        void ShowMouseCursor();
        void HideMouseCursor();

        void OnSDLEvent(SDL_Event& evnt);

        void LoadScheme(const std::string& schemeFile);
        void SetFont(const std::string& fontFile);
        CEGUI::Window* CreateWidget(const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
        static void SetWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPerc, const glm::vec4& destRectPix);

        // Getters
        static CEGUI::OpenGL3Renderer* GetRenderer() { return m_renderer; }
        const CEGUI::GUIContext* GetContext() { return m_context; }

    private:
        static CEGUI::OpenGL3Renderer* m_renderer;
        CEGUI::GUIContext* m_context = nullptr;
        CEGUI::Window* m_root = nullptr;
        unsigned int m_lastTime = 0;
    };
}
