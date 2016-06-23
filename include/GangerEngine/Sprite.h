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

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <GangerEngine/GLTexture.h>

#include <GL/glew.h>
#include <string>

namespace GangerEngine {
/// A 2D quad that can be rendered to the screen
class Sprite {
 public:
    Sprite();
    ~Sprite();

    void Init(float x, float y, float width, float height,
        std::string texturePath);

    void Draw();

    // private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    GLuint m_vboID;
    GLTexture m_texture;
};
}  // namespace GangerEngine

#endif  // _SPRITE_H_
