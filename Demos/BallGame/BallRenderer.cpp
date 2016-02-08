#include "BallRenderer.h"

void BallRenderer::renderBall(GangerEngine::SpriteBatch& spriteBatch, Ball& ball) {
    const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
    const glm::vec4 destRect(ball.position.x, ball.position.y,
                             ball.radius * 2.0f, ball.radius * 2.0f);
    spriteBatch.Draw(destRect, uvRect, ball.textureId, 0.0f, ball.color);
}
