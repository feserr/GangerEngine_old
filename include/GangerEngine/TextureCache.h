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

#ifndef _TEXTURECACHE_H_
#define _TEXTURECACHE_H_

#include <GangerEngine/GLTexture.h>

#include <string>
#include <map>

namespace GangerEngine {
// This caches the textures so that multiple sprites can use the same textures
class TextureCache {
 public:
    TextureCache();
    ~TextureCache();

    GLTexture GetTexture(std::string texturePath);

 private:
    std::map<std::string, GLTexture> m_textureMap;
};
}  // namespace GangerEngine

#endif  // _TEXTURECACHE_H_
