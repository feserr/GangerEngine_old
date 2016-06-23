/*
    Copyright [2016] [Ganger Games]

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _GUI_H_
#define _GUI_H_

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>
#include <glm/glm.hpp>
#include <SDL/SDL_events.h>

#include <string>

namespace GangerEngine {
class GUI {
 public:
    void Init(const std::string& resourceDirectory);
    void Destroy();

    void Draw();
    void Update();

    void SetMouseCursor(const std::string& imageFile);
    void ShowMouseCursor();
    void HideMouseCursor();

    void OnSDLEvent(const SDL_Event& evnt);

    void LoadScheme(const std::string& schemeFile);
    void SetFont(const std::string& fontFile);
    CEGUI::Window* CreateWidget(const std::string& type,
        const glm::vec4& destRectPerc, const glm::vec4& destRectPix,
        const std::string& name = "");
    CEGUI::Window* CreateWidget(CEGUI::Window* parent,
        const std::string& type, const glm::vec4& destRectPerc,
        const glm::vec4& destRectPix, const std::string& name = "");
    static void SetWidgetDestRect(CEGUI::Window* widget,
        const glm::vec4& destRectPerc, const glm::vec4& destRectPix);

    // Getters
    static CEGUI::OpenGL3Renderer* GetRenderer() { return m_renderer; }
    const CEGUI::GUIContext* GetContext() { return m_context; }

 private:
    static CEGUI::OpenGL3Renderer* m_renderer;
    CEGUI::GUIContext* m_context = nullptr;
    CEGUI::Window* m_root = nullptr;
    unsigned int m_lastTime = 0;
};
}  // namespace GangerEngine

#endif  // _GUI_H_
