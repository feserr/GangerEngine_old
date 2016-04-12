
#include <GangerEngine/ParticleEngine2D.h>

#include <GangerEngine/ParticleBatch2D.h>
#include <GangerEngine/SpriteBatch.h>

namespace GangerEngine
{
    ParticleEngine2D::ParticleEngine2D()
    {}

    ParticleEngine2D::~ParticleEngine2D()
    {
        for(auto b : m_batches)
            delete b;
    }

    void ParticleEngine2D::AddParticleBatch(ParticleBatch2D* particleBatch)
    {
        m_batches.push_back(particleBatch);
    }

    void ParticleEngine2D::Update(const float deltaTime)
    {
        for(auto& b : m_batches)
        {
            b->Update(deltaTime);
        }
    }

    void ParticleEngine2D::Draw(SpriteBatch* spriteBatch)
    {
        for(auto& b : m_batches)
        {
            spriteBatch->Begin();

            b->Draw(spriteBatch);

            spriteBatch->End();
            spriteBatch->RenderBatch();
        }
    }
}
