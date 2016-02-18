#pragma once

#include <GangerEngine/Window.h>
#include <GangerEngine/GLSLProgram.h>
#include <GangerEngine/Camera2D.h>
#include <GangerEngine/InputManager.h>
#include <GangerEngine/SpriteBatch.h>
#include <GangerEngine/SpriteFont.h>
#include <GangerEngine/AudioEngine.h>
#include <GangerEngine/ParticleEngine2D.h>
#include <GangerEngine/particleBatch2d.h>

#include "Player.h"
#include "Level.h"
#include "Bullet.h"

class Zombie;

enum class GameState {
    PLAY,
    EXIT
};

class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
    void initSystems();

    /// Initializes the level and sets up everything
    void initLevel();

    /// Initializes the music
    void initMusic();

    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();

    /// Updates all agents
    void updateAgents(float deltaTime);

    /// Updates all bullets
    void updateBullets(float deltaTime);

    /// Checks the victory condition
    void checkVictory();

    /// Handles input processing
    void processInput();

    /// Renders the game
    void drawGame();
    
    /// Draws the HUB
    void DrawHub();

    /// Adds blood
    void AddBlood(const glm::vec2& position , const int numParticles);

    /// Member Variables
    GangerEngine::Window m_window; ///< The game window
    
    GangerEngine::GLSLProgram m_textureProgram; ///< The shader program

    GangerEngine::InputManager m_inputManager; ///< Handles input

    GangerEngine::Camera2D m_camera; ///< Main Camera
    GangerEngine::Camera2D m_hubCamera;

    GangerEngine::SpriteBatch m_agentSpriteBatch; ///< Draws all agents
    GangerEngine::SpriteBatch m_hubSpriteBatch;

    std::vector<Level*> m_levels; ///< vector of all levels

    int m_screenWidth, m_screenHeight;

    float m_fps;

    int m_currentLevel;

    Player* m_player;
    std::vector<Human*> m_humans; ///< Vector of all humans
    std::vector<Zombie*> m_zombies; ///< Vector of all zombies
    std::vector<Bullet> m_bullets;

    int m_numHumansKilled; ///< Humans killed by player
    int m_numZombiesKilled; ///< Zombies killed by player
    
    GangerEngine::SpriteFont* m_spriteFont;
    
    GangerEngine::AudioEngine m_audioEngine;

    GangerEngine::ParticleEngine2D m_particleEngine;
    GangerEngine::ParticleBatch2D* m_bloodParticle;

    GameState m_gameState;
};

