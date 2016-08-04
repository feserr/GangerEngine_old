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

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Ball.h"

#include <GangerEngine/InputManager.h>
#include <GangerEngine/SpriteBatch.h>
#include <GangerEngine/TileSheet.h>
#include <GangerEngine/DebugRenderer.h>

enum class PlayerMoveState {STANDING, MOVING};
enum class PlayerType { PLAYERONE, PLAYERTWO, BOT};

class Player {
public:
    void Init(const glm::vec2& position, const glm::vec2& drawDims,
        glm::vec2& collisionDims, GangerEngine::ColorRGBA8 color,
        PlayerType playerType);

    void Destroy();

    void Draw(GangerEngine::SpriteBatch& spriteBatch);
    void DrawDebug(GangerEngine::DebugRenderer& debugRenderer);

    void Update(float deltaTime, GangerEngine::InputManager& inputManager,
        const glm::vec2 &windowSize, Ball &ball);

    glm::vec2 GetPosition() const {
        glm::vec2 rv;
        rv.x = m_position.x;
        rv.y = m_position.y;
        return rv;
    }

    const glm::vec2& GetDrawDims() const { return m_drawDims; }
    const glm::vec2& GetCollisionDims() const { return m_collisionDims; }
    const GangerEngine::ColorRGBA8& GetColor() const { return m_color; }

private:
    glm::vec2 m_drawDims;
    glm::vec2 m_collisionDims;
    GangerEngine::ColorRGBA8 m_color;
    GangerEngine::TileSheet m_texture;
    PlayerMoveState m_moveState = PlayerMoveState::STANDING;
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    PlayerType m_playerType;
};



#endif  // _PLAYER_H_
