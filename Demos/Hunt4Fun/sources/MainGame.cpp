#include "MainGame.h"

#include <GangerEngine/GangerEngine.h>
#include <GangerEngine/Timing.h>
#include <GangerEngine/GangerErrors.h>
#include <GangerEngine/ResourceManager.h>
#include <GangerEngine/Vertex.h>

#include <SDL/SDL.h>
#include <iostream>
#include <random>
#include <GLM/gtx/rotate_vector.hpp>

#include "Gun.h"
#include "Zombie.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame() :
    m_screenWidth(1024),
    m_screenHeight(768),
    m_gameState(GameState::PLAY),
    m_fps(0),
    m_player(nullptr),
    m_numHumansKilled(0),
    m_numZombiesKilled(0) {
    // Empty
}

MainGame::~MainGame() {
    // Don't forget to delete the levels!
    for (int i = 0; i < m_levels.size(); i++) {
        delete m_levels[i];
    }
    // Don't forget to delete the humans and zombies!
    for (int i = 0; i < m_humans.size(); i++) {
        delete m_humans[i];
    }
    for (int i = 0; i < m_zombies.size(); i++) {
        delete m_zombies[i];
    }
}

void MainGame::run() {

    initSystems();

    initLevel();

    initMusic();

    gameLoop();
}

void MainGame::initSystems() {
    // Initialize the game engine
    GangerEngine::Init();

    // Create our window
    m_window.Create("Hunt4Fun", m_screenWidth, m_screenHeight, 0);

    // Grey background color
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    // Set up the shaders
    initShaders();

    // Initialize our spritebatch
    m_agentSpriteBatch.Init();
    m_hubSpriteBatch.Init();

    // Initialize sprite font
    m_spriteFont = new GangerEngine::SpriteFont("Fonts/nokiafc22.ttf", 64);

    // Initialize audio engine
    m_audioEngine.Init();

    // Initialize particles
    m_bloodParticle = new GangerEngine::ParticleBatch2D();
    m_bloodParticle->Init(1000, 0.05f,
                          GangerEngine::ResourceManager::GetTexture("Textures/particle.png"));
    m_particleEngine.AddParticleBatch(m_bloodParticle);

    // Set up the camera
    m_camera.Init(m_screenWidth, m_screenHeight);
    
    // Set up the hub camera
    m_hubCamera.Init(m_screenWidth, m_screenHeight);
    
    // Set up the hub camera position
    glm::vec2 hubPosition(m_screenWidth / 2, m_screenHeight / 2);
    m_hubCamera.SetPosition(hubPosition);

}

void MainGame::initLevel() {
    // Level 1
    m_levels.push_back(new Level("Levels/level1.txt"));
    m_currentLevel = 0;

    m_player = new Player();
    m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager,
                  &m_camera, &m_bullets);

    m_humans.push_back(m_player);

    std::mt19937 randomEngine;
    randomEngine.seed((unsigned int)time(nullptr));

    std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
    std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);

    // Add all the random humans
    for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++) {
        m_humans.push_back(new Human);
        glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
        m_humans.back()->init(HUMAN_SPEED, pos);
    }

    // Add the zombies
    const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPositions();
    for (int i = 0; i < zombiePositions.size(); i++) {
        m_zombies.push_back(new Zombie);
        m_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
    }

    // Set up the players guns
    const float BULLET_SPEED = 20.0f;
    m_player->addGun(new Gun("Magnum", 10, 1, 0.1f, 30, BULLET_SPEED,
                            m_audioEngine.LoadSoundEffect("Sounds/shots/pistol.wav")));
    m_player->addGun(new Gun("Shotgun", 30, 12, 0.5f, 4, BULLET_SPEED,
                            m_audioEngine.LoadSoundEffect("Sounds/shots/shotgun.wav")));
    m_player->addGun(new Gun("MP5", 2, 1, 0.3f, 20, BULLET_SPEED,
                            m_audioEngine.LoadSoundEffect("Sounds/shots/cg1.wav")));
}

void MainGame::initMusic()
{
    GangerEngine::Music music = m_audioEngine.LoadMusic("Sounds/XYZ.ogg");
    music.Play();
}

void MainGame::initShaders() {
    // Compile our color shader
    m_textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    m_textureProgram.AddAttribute("vertexPosition");
    m_textureProgram.AddAttribute("vertexUV");
    m_textureProgram.AddAttribute("vertexColor");
    m_textureProgram.LinkShaders();
}

void MainGame::gameLoop() {
    
    GangerEngine::FpsLimiter fpsLimiter;
    fpsLimiter.SetTargetFPS(60.0f);
    
    float CAMERA_SCALE = 1.0f / 3.0f;
    m_camera.SetScale(CAMERA_SCALE);
    
    // Constant variable to handle delta time
    const float MS_PER_FRAME = 1000.0f;
    const float DESIREDm_fps = 60.0f;
    const float DESIRED_FRAMETIME = MS_PER_FRAME / DESIREDm_fps;
    const float MAX_DELTA_TIME = 1.0f;
    
    // Max steps of the simulationb
    const float MAX_PHYSICS_STEPS = 6;
    
    float previousTicks = SDL_GetTicks();

    // Main loop
    while (m_gameState == GameState::PLAY)
    {
        fpsLimiter.Begin();
        
        float newTicks = SDL_GetTicks();
        float frameTime = newTicks -  previousTicks;
        previousTicks = newTicks;
        float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

        checkVictory();

        processInput();
        
        int i = 0;
        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
        {
            float deltaTime = std:: min(totalDeltaTime, MAX_DELTA_TIME);
            
            updateAgents(deltaTime);
            updateBullets(deltaTime);

            m_particleEngine.Update(deltaTime);
            
            totalDeltaTime -= deltaTime;
            ++i;
        }
        
        glm::vec2 position = m_player->getPosition();

        m_camera.SetPosition(position);
        m_camera.Update();
        
        m_hubCamera.Update();

        drawGame();

        m_fps = fpsLimiter.End();
    }
}

void MainGame::updateAgents(float deltaTime) {
    // Update all humans
    for (int i = 0; i < m_humans.size(); i++) {
        m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),
                           m_humans,
                           m_zombies,
                           deltaTime);
    }

    // Update all zombies
    for (int i = 0; i < m_zombies.size(); i++) {
        m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(),
                           m_humans,
                           m_zombies,
                            deltaTime);
    }

    // Update Zombie collisions
    for (int i = 0; i < m_zombies.size(); i++) {
        // Collide with other zombies
        for (int j = i + 1; j < m_zombies.size(); j++) {
            m_zombies[i]->collideWithAgent(m_zombies[j]);
        }
        // Collide with humans
        for (int j = 1; j < m_humans.size(); j++) {
            if (m_zombies[i]->collideWithAgent(m_humans[j])) {
                // Add the new zombie
                m_zombies.push_back(new Zombie);
                m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
                // Delete the human
                delete m_humans[j];
                m_humans[j] = m_humans.back();
                m_humans.pop_back();
            }
        }

        // Collide with player
        if (m_zombies[i]->collideWithAgent(m_player)) {
            GangerEngine::FatalError("YOU LOSE");
        }
    }

    // Update Human collisions
    for (int i = 0; i < m_humans.size(); i++) {
        // Collide with other humans
        for (int j = i + 1; j < m_humans.size(); j++) {
            m_humans[i]->collideWithAgent(m_humans[j]);
        }
    }

    // Dont forget to update zombies
}

void MainGame::updateBullets(float deltaTime) {
    // Update and collide with world
    for (int i = 0; i < m_bullets.size(); ) {
        // If update returns true, the bullet collided with a wall
        if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime)) {
            m_bullets[i] = m_bullets.back();
            m_bullets.pop_back();
        } else {
            i++;
        }
    }

    bool wasBulletRemoved;

    // Collide with humans and zombies
    for (int i = 0; i < m_bullets.size(); i++) {
        wasBulletRemoved = false;
        // Loop through zombies
        for (int j = 0; j < m_zombies.size(); ) {
            // Check collision
            if (m_bullets[i].collideWithAgent(m_zombies[j])) {
                // Add blood
                AddBlood(m_bullets[i].GetPosition(), 5);

                // Damage zombie, and kill it if its out of health
                if (m_zombies[j]->applyDamage(m_bullets[i].getDamage())) {
                    // If the zombie died, remove him
                    delete m_zombies[j];
                    m_zombies[j] = m_zombies.back();
                    m_zombies.pop_back();
                    m_numZombiesKilled++;
                } else {
                    j++;
                }

                // Remove the bullet
                m_bullets[i] = m_bullets.back();
                m_bullets.pop_back();
                wasBulletRemoved = true;
                i--; // Make sure we don't skip a bullet
                // Since the bullet died, no need to loop through any more zombies
                break;
            } else {
                j++;
            }
        }
        // Loop through humans
        if (wasBulletRemoved == false) {
            for (int j = 1; j < m_humans.size(); ) {
                // Check collision
                if (m_bullets[i].collideWithAgent(m_humans[j])) {
                    // Add blood
                    AddBlood(m_bullets[i].GetPosition() , 5);

                    // Damage human, and kill it if its out of health
                    if (m_humans[j]->applyDamage(m_bullets[i].getDamage())) {
                        // If the human died, remove him
                        delete m_humans[j];
                        m_humans[j] = m_humans.back();
                        m_humans.pop_back();
                    } else {
                        j++;
                    }

                    // Remove the bullet
                    m_bullets[i] = m_bullets.back();
                    m_bullets.pop_back();
                    m_numHumansKilled++;
                    i--; // Make sure we don't skip a bullet
                    // Since the bullet died, no need to loop through any more zombies
                    break;
                } else {
                    j++;
                }
            }
        }
    }
}

void MainGame::checkVictory() {
    // TODO: Support for multiple levels!
    // m_currentLevel++; initLevel(...);

    // If all zombies are dead we win!
    if (m_zombies.empty()) {
        // Print victory message
        std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining",
                    m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_currentLevel]->getNumHumans());

        // Easy way to end the game :P
        GangerEngine::FatalError("");
    }
}

void MainGame::processInput() {
    SDL_Event evnt;
    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.PressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.ReleaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_inputManager.PressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_inputManager.ReleaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_textureProgram.use();

    // Draw code goes here
    glActiveTexture(GL_TEXTURE0);

    // Make sure the shader uses texture 0
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    glm::mat4 projectionMatrix = m_camera.GetCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Draw the level
    m_levels[m_currentLevel]->draw();

    // Begin drawing agents
    m_agentSpriteBatch.Begin();

    const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);
    
    // Draw the humans
    for (int i = 0; i < m_humans.size(); i++)
    {
        if(m_camera.IsBoxInView(m_humans[i]->getPosition(), agentDims))
            m_humans[i]->draw(m_agentSpriteBatch);
    }

    // Draw the zombies
    for (int i = 0; i < m_zombies.size(); i++)
    {
        if(m_camera.IsBoxInView(m_zombies[i]->getPosition(), agentDims))
            m_zombies[i]->draw(m_agentSpriteBatch);
    }

    // Draw the bullets
    for (int i = 0; i < m_bullets.size(); i++) {
        m_bullets[i].draw(m_agentSpriteBatch);
    }
    
    // End spritebatch creation
    m_agentSpriteBatch.End();

    // Render to the screen
    m_agentSpriteBatch.RenderBatch();

    // Render the particles
    m_particleEngine.Draw(&m_agentSpriteBatch);

    DrawHub();
    
    // Unbind the program
    m_textureProgram.unuse();

    // Swap our buffer and draw everything to the screen!
    m_window.Swap();
}

void MainGame::DrawHub()
{
    char buffer[256];
    
    // Grab the camera matrix
    glm::mat4 projectionMatrix = m_hubCamera.GetCameraMatrix();
    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
    
    m_hubSpriteBatch.Begin();
    
    std::sprintf(buffer, "Num. humans: %lu", m_humans.size());
    m_spriteFont->draw(m_hubSpriteBatch, buffer, glm::vec2(6, 0), glm::vec2(0.5f), 0.0f,
                      GangerEngine::ColorRGBA8(255, 255, 255, 255));

    std::sprintf(buffer, "Num. zombies: %lu", m_zombies.size());
    m_spriteFont->draw(m_hubSpriteBatch, buffer, glm::vec2(6, 36), glm::vec2(0.5f), 0.0f,
                      GangerEngine::ColorRGBA8(255, 255, 255, 255));

    
    m_hubSpriteBatch.End();
    m_hubSpriteBatch.RenderBatch();
}

void MainGame::AddBlood(const glm::vec2& position, int numParticles)
{
    static std::mt19937 randEngine(time(nullptr));
    static std::uniform_real_distribution<float> randAngle(0.0f, 360.0f);

    glm::vec2 velocity(2.0f, 0.0f);
    GangerEngine::ColorRGBA8 color(255, 0, 0, 255);

    for(int i = 0; i < numParticles; i++)
        m_bloodParticle->AddParticle(position, glm::rotate(velocity, randAngle(randEngine)), color,
                                     30.0f);
}