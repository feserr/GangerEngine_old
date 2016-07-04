//
//  Player.cpp
//  Shooting2Much
//
//  Created by Francisco Elias Serrano Martinez-Santos on 04/07/16.
//  Copyright © 2016 Ganger Games. All rights reserved.
//

#include "Player.h"

#include <GangerEngine/ResourceManager.h>
#include <SDL/SDL.h>
#include <iostream>

void Player::Init(const glm::vec2& position, const glm::vec2& drawDims,
    glm::vec2& collisionDims, GangerEngine::ColorRGBA8 color) {
    GangerEngine::GLTexture texture = GangerEngine::ResourceManager::GetTexture("Assets/blue_ninja.png");
    m_color = color;
    m_drawDims = drawDims;
    m_collisionDims = collisionDims;
    m_position = position;
    m_texture.Init(texture, glm::ivec2(10, 2));
}

void Player::Destroy() {
}

void Player::Draw(GangerEngine::SpriteBatch& spriteBatch) {
    glm::vec4 destRect;
    destRect.x = m_position.x - m_drawDims.x / 2.0f;
    destRect.y = m_position.y - m_drawDims.y / 2.0f;
    destRect.z = m_drawDims.x;
    destRect.w = m_drawDims.y;

    int tileIndex;
    int numTiles;

    float animSpeed = 0.2f;
    glm::vec2 velocity;
    velocity.x = m_position.x;
    velocity.y = m_position.y;

    m_onGround = true;

    // Calculate animation
    if (m_onGround) {

        if (m_isPunching) {
            numTiles = 4;
            tileIndex = 1;
            if (m_moveState != PlayerMoveState::PUNCHING) {
                m_moveState = PlayerMoveState::PUNCHING;
                m_animTime = 0.0f;
            }
        } else if (std::abs(velocity.x) > 1.0f && ((velocity.x > 0 && m_direction > 0) || (velocity.x < 0 && m_direction < 0))) {
            // Running
            numTiles = 6;
            tileIndex = 10;
            animSpeed = std::abs(velocity.x) * 0.025f;
            if (m_moveState != PlayerMoveState::RUNNING) {
                m_moveState = PlayerMoveState::RUNNING;
                m_animTime = 0.0f;
            }
        } else {
            // Standing still
            numTiles = 1;
            tileIndex = 0;
            m_moveState = PlayerMoveState::STANDING;
        }

    } else {
        // In the air
        if (m_isPunching) {
            numTiles = 1;
            tileIndex = 18;
            animSpeed *= 0.25f;
            if (m_moveState != PlayerMoveState::PUNCHING) {
                m_moveState = PlayerMoveState::PUNCHING;
                m_animTime = 0.0f;
            }
        } else if (std::abs(velocity.x) > 10.0f) {
            numTiles = 1;
            tileIndex = 10;
            m_moveState = PlayerMoveState::IN_AIR;
        } else if (velocity.y <= 0.0f) {
            // Falling
            numTiles = 1;
            tileIndex = 17;
            m_moveState = PlayerMoveState::IN_AIR;
        } else {
            // Rising
            numTiles = 1;
            tileIndex = 16;
            m_moveState = PlayerMoveState::IN_AIR;
        }
    }

    // Increment animation time
    m_animTime += animSpeed;

    // Check for punch end
    if (m_animTime > numTiles) {
        m_isPunching = false;
    }

    // Apply animation
    tileIndex = tileIndex + (int)m_animTime % numTiles;

    // Get the uv coordinates from the tile index
    glm::vec4 uvRect = m_texture.GetUVs(tileIndex);

    // Check direction
    if (m_direction == -1) {
        uvRect.x += 1.0f / m_texture.dims.x;
        uvRect.z *= -1;
    }

    // Draw the sprite
    spriteBatch.Draw(destRect, uvRect, m_texture.texture.id, 0.0f, m_color);
}

void Player::DrawDebug(GangerEngine::DebugRenderer& debugRenderer) {
    GangerEngine::ColorRGBA8 color(255, 255, 255, 255);
    // Draw box
    glm::vec4 destRect;
    destRect.x = m_position.x - m_drawDims.x / 2.0f;
    destRect.y = m_position.y - m_drawDims.y / 2.0f;
    destRect.z = m_drawDims.x;
    destRect.w = m_drawDims.y;
    debugRenderer.DrawBox(destRect, color, 0.0f);
}

void Player::Update(GangerEngine::InputManager& inputManager) {
    if (inputManager.IsKeyDown(SDLK_a)) {
        m_direction = -1;
        m_position.x -= 2;
    } else if (inputManager.IsKeyDown(SDLK_d)) {
        m_direction = 1;
        m_position.x += 2;
    } else if (inputManager.IsKeyDown(SDLK_w)) {
        m_position.y += 2;
    } else if (inputManager.IsKeyDown(SDLK_s)) {
        m_position.y -= 2;
    } else {
    }

    // Check for punch
    if (inputManager.IsKeyPressed(SDLK_SPACE)) {
        m_isPunching = true;
    }

    float MAX_SPEED = 10.0f;

    // Loop through all the contact points
    m_onGround = false;

    printf("%2.fx%2.f\n", m_position.x, m_position.y);
}
