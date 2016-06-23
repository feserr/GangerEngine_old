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

#include <GL/glew.h>  // Include BEFORE GUI.h

#include <GangerEngine/GUI.h>
#include <SDL/SDL_timer.h>
#include <utf8/utf8.h>

#include <string>
#include <vector>
#include <iostream>


namespace GangerEngine {
    CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;

    void GUI::Init(const std::string& resourceDirectory) {
        // Check if the renderer and system were not already initialized
        if (m_renderer == nullptr) {
            m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
        }

        CEGUI::DefaultResourceProvider* rp =
            static_cast<CEGUI::DefaultResourceProvider*>(
                CEGUI::System::getSingleton().getResourceProvider());
        rp->setResourceGroupDirectory("imagesets", resourceDirectory +
            "/imagesets/");
        rp->setResourceGroupDirectory("schemes", resourceDirectory +
            "/schemes/");
        rp->setResourceGroupDirectory("fonts", resourceDirectory +
            "/fonts/");
        rp->setResourceGroupDirectory("layouts", resourceDirectory +
            "/layouts/");
        rp->setResourceGroupDirectory("looknfeels", resourceDirectory +
            "/looknfeel/");
        rp->setResourceGroupDirectory("lua_scripts", resourceDirectory +
            "/lua_scripts/");

        CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
        CEGUI::Scheme::setDefaultResourceGroup("schemes");
        CEGUI::Font::setDefaultResourceGroup("fonts");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
        CEGUI::WindowManager::setDefaultResourceGroup("layouts");
        CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

        m_context = &CEGUI::System::getSingleton()
            .createGUIContext(m_renderer->getDefaultRenderTarget());
        m_root = CEGUI::WindowManager::getSingleton()
            .createWindow("DefaultWindow", "root");
        m_context->setRootWindow(m_root);
    }

    void GUI::Destroy() {
        CEGUI::System::getSingleton().destroyGUIContext(*m_context);
        CEGUI::WindowManager::getSingleton().destroyWindow(m_root);
        m_context = nullptr;
        m_root = nullptr;
    }

    void GUI::Draw() {
        glDisable(GL_DEPTH_TEST);
        m_renderer->beginRendering();
        m_context->draw();
        m_renderer->endRendering();
        // Clean up after CEGUI
        glBindVertexArray(0);
        glDisable(GL_SCISSOR_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    void GUI::Update() {
        unsigned int elapsed;
        if (m_lastTime == 0) {
            elapsed = 0;
            m_lastTime = SDL_GetTicks();
        } else {
            unsigned int nextTime = SDL_GetTicks();
            elapsed = nextTime - m_lastTime;
            m_lastTime = nextTime;
        }

        m_context->injectTimePulse(static_cast<float>(elapsed) / 1000.0f);
    }

    void GUI::SetMouseCursor(const std::string& imageFile) {
        m_context->getMouseCursor().setDefaultImage(imageFile);
    }

    void GUI::ShowMouseCursor() {
        m_context->getMouseCursor().show();
    }

    void GUI::HideMouseCursor() {
        m_context->getMouseCursor().hide();
    }

    CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key) {
        switch (key) {
            case SDLK_BACKSPACE:    return CEGUI::Key::Backspace;
            case SDLK_TAB:          return CEGUI::Key::Tab;
            case SDLK_RETURN:       return CEGUI::Key::Return;
            case SDLK_PAUSE:        return CEGUI::Key::Pause;
            case SDLK_ESCAPE:       return CEGUI::Key::Escape;
            case SDLK_SPACE:        return CEGUI::Key::Space;
            case SDLK_COMMA:        return CEGUI::Key::Comma;
            case SDLK_MINUS:        return CEGUI::Key::Minus;
            case SDLK_PERIOD:       return CEGUI::Key::Period;
            case SDLK_SLASH:        return CEGUI::Key::Slash;
            case SDLK_0:            return CEGUI::Key::Zero;
            case SDLK_1:            return CEGUI::Key::One;
            case SDLK_2:            return CEGUI::Key::Two;
            case SDLK_3:            return CEGUI::Key::Three;
            case SDLK_4:            return CEGUI::Key::Four;
            case SDLK_5:            return CEGUI::Key::Five;
            case SDLK_6:            return CEGUI::Key::Six;
            case SDLK_7:            return CEGUI::Key::Seven;
            case SDLK_8:            return CEGUI::Key::Eight;
            case SDLK_9:            return CEGUI::Key::Nine;
            case SDLK_COLON:        return CEGUI::Key::Colon;
            case SDLK_SEMICOLON:    return CEGUI::Key::Semicolon;
            case SDLK_EQUALS:       return CEGUI::Key::Equals;
            case SDLK_LEFTBRACKET:  return CEGUI::Key::LeftBracket;
            case SDLK_BACKSLASH:    return CEGUI::Key::Backslash;
            case SDLK_RIGHTBRACKET: return CEGUI::Key::RightBracket;
            case SDLK_a:            return CEGUI::Key::A;
            case SDLK_b:            return CEGUI::Key::B;
            case SDLK_c:            return CEGUI::Key::C;
            case SDLK_d:            return CEGUI::Key::D;
            case SDLK_e:            return CEGUI::Key::E;
            case SDLK_f:            return CEGUI::Key::F;
            case SDLK_g:            return CEGUI::Key::G;
            case SDLK_h:            return CEGUI::Key::H;
            case SDLK_i:            return CEGUI::Key::I;
            case SDLK_j:            return CEGUI::Key::J;
            case SDLK_k:            return CEGUI::Key::K;
            case SDLK_l:            return CEGUI::Key::L;
            case SDLK_m:            return CEGUI::Key::M;
            case SDLK_n:            return CEGUI::Key::N;
            case SDLK_o:            return CEGUI::Key::O;
            case SDLK_p:            return CEGUI::Key::P;
            case SDLK_q:            return CEGUI::Key::Q;
            case SDLK_r:            return CEGUI::Key::R;
            case SDLK_s:            return CEGUI::Key::S;
            case SDLK_t:            return CEGUI::Key::T;
            case SDLK_u:            return CEGUI::Key::U;
            case SDLK_v:            return CEGUI::Key::V;
            case SDLK_w:            return CEGUI::Key::W;
            case SDLK_x:            return CEGUI::Key::X;
            case SDLK_y:            return CEGUI::Key::Y;
            case SDLK_z:            return CEGUI::Key::Z;
            case SDLK_DELETE:       return CEGUI::Key::Delete;
            case SDLK_KP_PERIOD:    return CEGUI::Key::Decimal;
            case SDLK_KP_DIVIDE:    return CEGUI::Key::Divide;
            case SDLK_KP_MULTIPLY:  return CEGUI::Key::Multiply;
            case SDLK_KP_MINUS:     return CEGUI::Key::Subtract;
            case SDLK_KP_PLUS:      return CEGUI::Key::Add;
            case SDLK_KP_ENTER:     return CEGUI::Key::NumpadEnter;
            case SDLK_KP_EQUALS:    return CEGUI::Key::NumpadEquals;
            case SDLK_UP:           return CEGUI::Key::ArrowUp;
            case SDLK_DOWN:         return CEGUI::Key::ArrowDown;
            case SDLK_RIGHT:        return CEGUI::Key::ArrowRight;
            case SDLK_LEFT:         return CEGUI::Key::ArrowLeft;
            case SDLK_INSERT:       return CEGUI::Key::Insert;
            case SDLK_HOME:         return CEGUI::Key::Home;
            case SDLK_END:          return CEGUI::Key::End;
            case SDLK_PAGEUP:       return CEGUI::Key::PageUp;
            case SDLK_PAGEDOWN:     return CEGUI::Key::PageDown;
            case SDLK_F1:           return CEGUI::Key::F1;
            case SDLK_F2:           return CEGUI::Key::F2;
            case SDLK_F3:           return CEGUI::Key::F3;
            case SDLK_F4:           return CEGUI::Key::F4;
            case SDLK_F5:           return CEGUI::Key::F5;
            case SDLK_F6:           return CEGUI::Key::F6;
            case SDLK_F7:           return CEGUI::Key::F7;
            case SDLK_F8:           return CEGUI::Key::F8;
            case SDLK_F9:           return CEGUI::Key::F9;
            case SDLK_F10:          return CEGUI::Key::F10;
            case SDLK_F11:          return CEGUI::Key::F11;
            case SDLK_F12:          return CEGUI::Key::F12;
            case SDLK_F13:          return CEGUI::Key::F13;
            case SDLK_F14:          return CEGUI::Key::F14;
            case SDLK_F15:          return CEGUI::Key::F15;
            case SDLK_RSHIFT:       return CEGUI::Key::RightShift;
            case SDLK_LSHIFT:       return CEGUI::Key::LeftShift;
            case SDLK_RCTRL:        return CEGUI::Key::RightControl;
            case SDLK_LCTRL:        return CEGUI::Key::LeftControl;
            case SDLK_RALT:         return CEGUI::Key::RightAlt;
            case SDLK_LALT:         return CEGUI::Key::LeftAlt;
            case SDLK_SYSREQ:       return CEGUI::Key::SysRq;
            case SDLK_MENU:         return CEGUI::Key::AppMenu;
            case SDLK_POWER:        return CEGUI::Key::Power;
            default:                return CEGUI::Key::Unknown;
        }
    }

    CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton) {
        switch (sdlButton) {
            case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
            case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
            case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
            case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
            case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
        }
        return CEGUI::MouseButton::NoButton;
    }

    void GUI::OnSDLEvent(const SDL_Event& evnt) {
        CEGUI::utf32 codePoint;
        std::string evntText = std::string(evnt.text.text);
        std::vector<int> utf32result;
        switch (evnt.type) {
            case SDL_MOUSEMOTION:
                m_context->injectMousePosition(
                    static_cast<float>(evnt.motion.x),
                    static_cast<float>(evnt.motion.y));
                break;
            case SDL_KEYDOWN:
                m_context->injectKeyDown(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
                break;
            case SDL_KEYUP:
                m_context->injectKeyUp(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
                break;
            case SDL_TEXTINPUT:
                codePoint = 0;
                utf8::utf8to32(evnt.text.text, evnt.text.text + evntText.size(),
                    std::back_inserter(utf32result));
                codePoint = (CEGUI::utf32)utf32result[0];
                m_context->injectChar(codePoint);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_context->injectMouseButtonDown(SDLButtonToCEGUIButton(
                    evnt.button.button));
                break;
            case SDL_MOUSEBUTTONUP:
                m_context->injectMouseButtonUp(SDLButtonToCEGUIButton(
                    evnt.button.button));
                break;
        }
    }

    void GUI::LoadScheme(const std::string& schemeFile) {
        CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
    }

    CEGUI::Window* GUI::CreateWidget(const std::string& type,
        const glm::vec4& destRectPerc, const glm::vec4& destRectPix,
        const std::string& name /*= ""*/) {
        CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton()
            .createWindow(type, name);
        m_root->addChild(newWindow);
        SetWidgetDestRect(newWindow, destRectPerc, destRectPix);
        return newWindow;
    }

    CEGUI::Window* GUI::CreateWidget(CEGUI::Window* parent,
        const std::string& type, const glm::vec4& destRectPerc,
        const glm::vec4& destRectPix, const std::string& name /*= ""*/) {
        CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton()
            .createWindow(type, name);
        parent->addChild(newWindow);
        SetWidgetDestRect(newWindow, destRectPerc, destRectPix);
        return newWindow;
    }

    void GUI::SetWidgetDestRect(CEGUI::Window* widget,
        const glm::vec4& destRectPerc, const glm::vec4& destRectPix) {
        widget->setPosition(CEGUI::UVector2(CEGUI::UDim(
            destRectPerc.x, destRectPix.x), CEGUI::UDim(
                destRectPerc.y, destRectPix.y)));
        widget->setSize(CEGUI::USize(CEGUI::UDim(
            destRectPerc.z, destRectPix.z), CEGUI::UDim(
                destRectPerc.w, destRectPix.w)));
    }

    void GUI::SetFont(const std::string& fontFile) {
        CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
        m_context->setDefaultFont(fontFile);
    }
}  // namespace GangerEngine
