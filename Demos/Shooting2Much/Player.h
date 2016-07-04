//
//  Player.hpp
//  Shooting2Much
//
//  Created by Francisco Elias Serrano Martinez-Santos on 04/07/16.
//  Copyright © 2016 Ganger Games. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <GangerEngine/InputManager.h>
#include <GangerEngine/SpriteBatch.h>
#include <GangerEngine/TileSheet.h>
#include <GangerEngine/DebugRenderer.h>

enum class PlayerMoveState {STANDING, RUNNING, PUNCHING, IN_AIR };

class Player {
public:
    void Init(const glm::vec2& position,
              const glm::vec2& drawDims,
              glm::vec2& collisionDims,
              GangerEngine::ColorRGBA8 color);

    void Destroy();

    void Draw(GangerEngine::SpriteBatch& spriteBatch);
    void DrawDebug(GangerEngine::DebugRenderer& debugRenderer);

    void Update(GangerEngine::InputManager& inputManager);

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
    float m_animTime = 0.0f;
    int m_direction = 1; // 1 or -1
    bool m_onGround = false;
    bool m_isPunching = false;
};



#endif /* Player_h */
