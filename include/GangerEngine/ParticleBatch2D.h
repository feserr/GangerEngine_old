// ParticleBatch2D.h

#pragma once

#include <functional>
#include <GLM/glm.hpp>
#include <GangerEngine/Vertex.h>
#include <GangerEngine/SpriteBatch.h>
#include <GangerEngine/GLTexture.h>

namespace GangerEngine {
    class Particle2D
    {
    public:
        glm::vec4 GetDestRect() const;

        glm::vec2 position = glm::vec2(0.0f);
        glm::vec2 velocity = glm::vec2(0.0f);
        ColorRGBA8 color;
        float life = 0.0f;
        float width = 0.0f;
    };

    inline  void DefaultParticle2DUpdate(Particle2D& particle, const float deltaTime)
    {
        particle.position += particle.velocity * deltaTime ;
    }

    class ParticleBatch2D
    {
    public:
        ParticleBatch2D();
        ~ParticleBatch2D();

        void Init(const int maxParticles, const float decayRate,
                  const GLTexture texture,
                  std::function<void(Particle2D&, const float)> updateFunc = DefaultParticle2DUpdate);

        void AddParticle(const glm::vec2& position, const glm::vec2& velocity,
                         const ColorRGBA8& color, const float width);

        void Update(const float deltaTime);
        void Draw(SpriteBatch* spriteBatch);

    private:
        int FindFreeParticle();

        std::function<void(Particle2D&, const float)> m_updateFunc;

        float m_decayRate = 0.1f;
        Particle2D* m_particles = nullptr;
        int m_maxParticles = 0;
        int m_lastFreeParticle = 0;

        GLTexture m_texture;
    };
}
