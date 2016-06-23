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
    _screenWidth(1024),
    _screenHeight(768),
    _gameState(GameState::PLAY),
    _fps(0),
    _player(nullptr),
    _numHumansKilled(0),
    _numZombiesKilled(0) {
    // Empty
}

MainGame::~MainGame() {
    // Don't forget to delete the levels!
    for (int i = 0; i < _levels.size(); i++) {
        delete _levels[i];
    }
    // Don't forget to delete the humans and zombies!
    for (int i = 0; i < _humans.size(); i++) {
        delete _humans[i];
    }
    for (int i = 0; i < _zombies.size(); i++) {
        delete _zombies[i];
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
    _window.Create("ZombieGame", _screenWidth, _screenHeight, 0);

    // Grey background color
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

    // Set up the shaders
    initShaders();

    // Initialize our spritebatch
    _agentSpriteBatch.Init();
    _hubSpriteBatch.Init();

    // Initialize sprite font
    _spriteFont = new GangerEngine::SpriteFont("Fonts/nokiafc22.ttf", 64);

    // Initialize audio engine
    m_audioEngine.Init();

    // Initialize particles
    m_bloodParticle = new GangerEngine::ParticleBatch2D();
    m_bloodParticle->Init(1000, 0.05f,
                          GangerEngine::ResourceManager::GetTexture("Textures/particle.png"));
    m_particleEngine.AddParticleBatch(m_bloodParticle);

    // Set up the camera
    _camera.Init(_screenWidth, _screenHeight);
    
    // Set up the hub camera
    _hubCamera.Init(_screenWidth, _screenHeight);
    
    // Set up the hub camera position
    glm::vec2 hubPosition(_screenWidth / 2, _screenHeight / 2);
    _hubCamera.SetPosition(hubPosition);

}

void MainGame::initLevel() {
    // Level 1
    _levels.push_back(new Level("Levels/level1.txt"));
    _currentLevel = 0;

    _player = new Player();
    _player->init(PLAYER_SPEED, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager,
                  &_camera, &_bullets);

    _humans.push_back(_player);

    std::mt19937 randomEngine;
    randomEngine.seed((unsigned int)time(nullptr));

    std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 2);
    std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);

    // Add all the random humans
    for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++) {
        _humans.push_back(new Human);
        glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
        _humans.back()->init(HUMAN_SPEED, pos);
    }

    // Add the zombies
    const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPositions();
    for (int i = 0; i < zombiePositions.size(); i++) {
        _zombies.push_back(new Zombie);
        _zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
    }

    // Set up the players guns
    const float BULLET_SPEED = 20.0f;
    _player->addGun(new Gun("Magnum", 10, 1, 0.1f, 30, BULLET_SPEED,
                            m_audioEngine.LoadSoundEffect("Sounds/shots/pistol.wav")));
    _player->addGun(new Gun("Shotgun", 30, 12, 0.5f, 4, BULLET_SPEED,
                            m_audioEngine.LoadSoundEffect("Sounds/shots/shotgun.wav")));
    _player->addGun(new Gun("MP5", 2, 1, 0.3f, 20, BULLET_SPEED,
                            m_audioEngine.LoadSoundEffect("Sounds/shots/cg1.wav")));
}

void MainGame::initMusic()
{
    GangerEngine::Music music = m_audioEngine.LoadMusic("Sounds/XYZ.ogg");
    music.Play();
}

void MainGame::initShaders() {
    // Compile our color shader
    _textureProgram.CompileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
    _textureProgram.AddAttribute("vertexPosition");
    _textureProgram.AddAttribute("vertexUV");
    _textureProgram.AddAttribute("vertexColor");
    _textureProgram.LinkShaders();
}

void MainGame::gameLoop() {
    
    GangerEngine::FpsLimiter fpsLimiter;
    fpsLimiter.SetMaxFPS(60.0f);
    
    float CAMERA_SCALE = 1.0f / 3.0f;
    _camera.SetScale(CAMERA_SCALE);
    
    // Constant variable to handle delta time
    const float MS_PER_FRAME = 1000.0f;
    const float DESIRED_FPS = 60.0f;
    const float DESIRED_FRAMETIME = MS_PER_FRAME / DESIRED_FPS;
    const float MAX_DELTA_TIME = 1.0f;
    
    // Max steps of the simulationb
    const float MAX_PHYSICS_STEPS = 6;
    
    float previousTicks = SDL_GetTicks();

    // Main loop
    while (_gameState == GameState::PLAY)
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
        
        glm::vec2 position = _player->getPosition();

        _camera.SetPosition(position);
        _camera.Update();
        
        _hubCamera.Update();

        drawGame();

        _fps = fpsLimiter.End();
    }
}

void MainGame::updateAgents(float deltaTime) {
    // Update all humans
    for (int i = 0; i < _humans.size(); i++) {
        _humans[i]->update(_levels[_currentLevel]->getLevelData(),
                           _humans,
                           _zombies,
                           deltaTime);
    }

    // Update all zombies
    for (int i = 0; i < _zombies.size(); i++) {
        _zombies[i]->update(_levels[_currentLevel]->getLevelData(),
                           _humans,
                           _zombies,
                            deltaTime);
    }

    // Update Zombie collisions
    for (int i = 0; i < _zombies.size(); i++) {
        // Collide with other zombies
        for (int j = i + 1; j < _zombies.size(); j++) {
            _zombies[i]->collideWithAgent(_zombies[j]);
        }
        // Collide with humans
        for (int j = 1; j < _humans.size(); j++) {
            if (_zombies[i]->collideWithAgent(_humans[j])) {
                // Add the new zombie
                _zombies.push_back(new Zombie);
                _zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
                // Delete the human
                delete _humans[j];
                _humans[j] = _humans.back();
                _humans.pop_back();
            }
        }

        // Collide with player
        if (_zombies[i]->collideWithAgent(_player)) {
            GangerEngine::FatalError("YOU LOSE");
        }
    }

    // Update Human collisions
    for (int i = 0; i < _humans.size(); i++) {
        // Collide with other humans
        for (int j = i + 1; j < _humans.size(); j++) {
            _humans[i]->collideWithAgent(_humans[j]);
        }
    }

    // Dont forget to update zombies
}

void MainGame::updateBullets(float deltaTime) {
    // Update and collide with world
    for (int i = 0; i < _bullets.size(); ) {
        // If update returns true, the bullet collided with a wall
        if (_bullets[i].update(_levels[_currentLevel]->getLevelData(), deltaTime)) {
            _bullets[i] = _bullets.back();
            _bullets.pop_back();
        } else {
            i++;
        }
    }

    bool wasBulletRemoved;

    // Collide with humans and zombies
    for (int i = 0; i < _bullets.size(); i++) {
        wasBulletRemoved = false;
        // Loop through zombies
        for (int j = 0; j < _zombies.size(); ) {
            // Check collision
            if (_bullets[i].collideWithAgent(_zombies[j])) {
                // Add blood
                AddBlood(_bullets[i].GetPosition(), 5);

                // Damage zombie, and kill it if its out of health
                if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
                    // If the zombie died, remove him
                    delete _zombies[j];
                    _zombies[j] = _zombies.back();
                    _zombies.pop_back();
                    _numZombiesKilled++;
                } else {
                    j++;
                }

                // Remove the bullet
                _bullets[i] = _bullets.back();
                _bullets.pop_back();
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
            for (int j = 1; j < _humans.size(); ) {
                // Check collision
                if (_bullets[i].collideWithAgent(_humans[j])) {
                    // Add blood
                    AddBlood(_bullets[i].GetPosition() , 5);

                    // Damage human, and kill it if its out of health
                    if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
                        // If the human died, remove him
                        delete _humans[j];
                        _humans[j] = _humans.back();
                        _humans.pop_back();
                    } else {
                        j++;
                    }

                    // Remove the bullet
                    _bullets[i] = _bullets.back();
                    _bullets.pop_back();
                    _numHumansKilled++;
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
    // _currentLevel++; initLevel(...);

    // If all zombies are dead we win!
    if (_zombies.empty()) {
        // Print victory message
        std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining",
                    _numHumansKilled, _numZombiesKilled, _humans.size() - 1, _levels[_currentLevel]->getNumHumans());

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
                _gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.SetMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                _inputManager.PressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                _inputManager.ReleaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                _inputManager.PressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                _inputManager.ReleaseKey(evnt.button.button);
                break;
        }
    }
}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _textureProgram.Use();

    // Draw code goes here
    glActiveTexture(GL_TEXTURE0);

    // Make sure the shader uses texture 0
    GLint textureUniform = _textureProgram.GetUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    // Grab the camera matrix
    glm::mat4 projectionMatrix = _camera.GetCameraMatrix();
    GLint pUniform = _textureProgram.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Draw the level
    _levels[_currentLevel]->draw();

    // Begin drawing agents
    _agentSpriteBatch.Begin();

    const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);
    
    // Draw the humans
    for (int i = 0; i < _humans.size(); i++)
    {
        if(_camera.IsBoxInView(_humans[i]->getPosition(), agentDims))
            _humans[i]->draw(_agentSpriteBatch);
    }

    // Draw the zombies
    for (int i = 0; i < _zombies.size(); i++)
    {
        if(_camera.IsBoxInView(_zombies[i]->getPosition(), agentDims))
            _zombies[i]->draw(_agentSpriteBatch);
    }

    // Draw the bullets
    for (int i = 0; i < _bullets.size(); i++) {
        _bullets[i].draw(_agentSpriteBatch);
    }
    
    // End spritebatch creation
    _agentSpriteBatch.End();

    // Render to the screen
    _agentSpriteBatch.RenderBatch();

    // Render the particles
    m_particleEngine.Draw(&_agentSpriteBatch);

    DrawHub();
    
    // Unbind the program
    _textureProgram.Unuse();

    // Swap our buffer and draw everything to the screen!
    _window.SwapBuffer();
}

void MainGame::DrawHub()
{
    char buffer[256];
    
    // Grab the camera matrix
    glm::mat4 projectionMatrix = _hubCamera.GetCameraMatrix();
    GLint pUniform = _textureProgram.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
    
    _hubSpriteBatch.Begin();
    
    std::sprintf(buffer, "Num. humans: %lu", _humans.size());
    _spriteFont->Draw(_hubSpriteBatch, buffer, glm::vec2(6, 0), glm::vec2(0.5f), 0.0f,
                      GangerEngine::ColorRGBA8(255, 255, 255, 255));

    std::sprintf(buffer, "Num. zombies: %lu", _zombies.size());
    _spriteFont->Draw(_hubSpriteBatch, buffer, glm::vec2(6, 36), glm::vec2(0.5f), 0.0f,
                      GangerEngine::ColorRGBA8(255, 255, 255, 255));

    
    _hubSpriteBatch.End();
    _hubSpriteBatch.RenderBatch();
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