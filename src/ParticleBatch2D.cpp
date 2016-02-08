// ParticleBatch2D.cpp

#include <GangerEngine/ParticleBatch2D.h>

namespace GangerEngine
{
    glm::vec4 Particle2D::GetDestRect() const
    {
        return glm::vec4(position.x, position.y, width, width);
    }

    ParticleBatch2D::ParticleBatch2D()
    {}

    ParticleBatch2D::~ParticleBatch2D()
    {
        delete[] m_particles;
    }

    void ParticleBatch2D::Init(const int maxParticles, const float decayRate,
                               const GLTexture texture,
                               std::function<void(Particle2D&, const float)> updateFunc
                                /* = DefaultParticle2DUpdate */)
    {
        m_maxParticles = maxParticles;
        m_decayRate = decayRate;
        m_texture = texture;
        m_particles = new Particle2D[m_maxParticles];
        m_updateFunc = updateFunc;
    }

    void ParticleBatch2D::AddParticle(const glm::vec2& position, const glm::vec2& velocity,
                                      const ColorRGBA8& color, const float width)
    {
        int particleIndex = FindFreeParticle();
        auto& p = m_particles[particleIndex];

        p.life = 1.0f;
        p.position = position;
        p.velocity = velocity;
        p.color = color;
        p.width = width;
    }

    void ParticleBatch2D::Update(const float deltaTime)
    {
        for (int i = 0; i < m_maxParticles; i++)
        {
            if(m_particles[i].life > 0.0f)
            {
                m_updateFunc(m_particles[i], deltaTime); 
                m_particles[i].life -= m_decayRate * deltaTime;
            }
        }
    }

    void ParticleBatch2D::Draw(SpriteBatch* spriteBatch)
    {
        glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

        for (int i = 0; i < m_maxParticles; i++)
        {
            auto p = m_particles[i];

            if(p.life > 0.0f)
            {
                spriteBatch->Draw(p.GetDestRect(), uvRect, m_texture.id, 0.0f, p.color);
            }
        }
    }

    int ParticleBatch2D::FindFreeParticle()
    {
        for(int i = m_lastFreeParticle; i < m_maxParticles; i++)
        {
            if(m_particles[i].life <= 0.0f)
            {
                m_lastFreeParticle = i;
                return i;
            }
        }

        for(int i = 0; i < m_lastFreeParticle ; i++)
        {
            if(m_particles[i].life <= 0.0f)
            {
                m_lastFreeParticle = i;
                return i;
            }
        }

        // No particles free
        return 0;
    }
}