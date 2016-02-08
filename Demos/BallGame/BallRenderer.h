#pragma once

#include <GangerEngine/SpriteBatch.h>
#include "Ball.h"

class BallRenderer {
public:
    void renderBall(GangerEngine::SpriteBatch& spriteBatch, Ball& ball);
};
