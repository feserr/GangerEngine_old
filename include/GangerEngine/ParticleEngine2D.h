// ParticleEngine2D.h

#pragma once

#include <vector>

namespace GangerEngine
{
    class ParticleBatch2D;
    class SpriteBatch;

    class ParticleEngine2D
    {
    public:
        ParticleEngine2D();
        ~ParticleEngine2D();

        // After adding a particle batch, the ParticleEngine2D becomes
        // responsible for deallocation.
        void AddParticleBatch(ParticleBatch2D* particleBatch);

        void Update(const float deltaTime);
        void Draw(SpriteBatch* spriteBatch);

    private:
        std::vector<ParticleBatch2D*> m_batches;
    };
}
