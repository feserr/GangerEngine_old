/*
    Copyright [2016] [Elías Serrano]

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
/// The GUI class
class GUI {
 public:
    /**
     * \brief      Initialize the GUI.
     *
     * \param[in]  resourceDirectory  The resource directory
     */
    void Init(const std::string& resourceDirectory);

    /// Destroy the GUI.
    void Destroy();

    /// Draws the GUI.
    void Draw();

    /// Updates the GUI.
    void Update();

    /**
     * \brief      Sets the mouse cursor.
     *
     * \param[in]  imageFile  The image file
     */
    void SetMouseCursor(const std::string& imageFile);

    /// Shows the mouse cursor.
    void ShowMouseCursor();

    /// Hides the mouse cursor.
    void HideMouseCursor();

    /**
     * \brief      Handle SDL events.
     *
     * \param[in]  evnt  The event
     */
    void OnSDLEvent(const SDL_Event& evnt);

    /**
     * \brief      Loads a scheme.
     *
     * \param[in]  schemeFile  The scheme file
     */
    void LoadScheme(const std::string& schemeFile);

    /**
     * \brief      Sets the font.
     *
     * \param[in]  fontFile  The font file
     */
    void SetFont(const std::string& fontFile);

    void SetSize(int width, int height) {
        CEGUI::System::getSingleton().getRenderer()->setDisplaySize(
            CEGUI::Sizef(width, height));
    }

    /**
     * \brief      Creates a widget.
     *
     * \param[in]  type          The type
     * \param[in]  destRectPerc  The destination rectangle perc
     * \param[in]  destRectPix   The destination rectangle pix
     * \param[in]  name          The name
     *
     * \return     Returns a pointer to the widget created.
     */
    CEGUI::Window* CreateWidget(const std::string& type,
        const glm::vec4& destRectPerc, const glm::vec4& destRectPix,
        const std::string& name = "");
    CEGUI::Window* CreateWidget(CEGUI::Window* parent,
        const std::string& type, const glm::vec4& destRectPerc,
        const glm::vec4& destRectPix, const std::string& name = "");
    static void SetWidgetDestRect(CEGUI::Window* widget,
        const glm::vec4& destRectPerc, const glm::vec4& destRectPix);

    /**
     * \brief      Gets the renderer.
     *
     * \return     The renderer.
     */
    static CEGUI::OpenGL3Renderer* GetRenderer() { return m_renderer; }

    /**
     * \brief      Gets the context.
     *
     * \return     The context.
     */
    const CEGUI::GUIContext* GetContext() { return m_context; }

 private:
    static CEGUI::OpenGL3Renderer* m_renderer;
    CEGUI::GUIContext* m_context = nullptr;
    CEGUI::Window* m_root = nullptr;
    unsigned int m_lastTime = 0;
};
}  // namespace GangerEngine

#endif  // _GUI_H_
