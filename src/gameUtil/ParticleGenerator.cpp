#include "ParticleGenerator.hpp"

ParticleGenerator::ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount)
    : m_shader{ shader }
    , m_texture{ texture }
    , m_amount{ amount }
    , m_vao{ 0 }
    , m_lastUsedParticle{ 0 }
{
    unsigned int vbo{ 0 };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * particleVertices.size(), particleVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    for(unsigned int i{ 0 }; i < m_amount; ++i)
        m_particles.emplace_back();
}

void ParticleGenerator::update(float dt, const GameObject& object, unsigned int newParticles, const glm::vec2& offset)
{
    for(unsigned int i{ 0 }; i < newParticles; ++i)
    {
        unsigned int unusedParticle{ firstUnusedParticle() };
        respawnParticle(m_particles.at(unusedParticle), object, offset);
    }

    for(unsigned int i{ 0 }; i < m_amount; ++i)
    {
        Particle& p{ m_particles.at(i) };
        p.life -= dt;

        if(p.life > 0.f)
        {
            p.position -= p.velocity * dt;
            p.color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::draw()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    m_shader.use();

    for(Particle p : m_particles)
    {
        if(p.life > 0.f)
        {
            m_shader.setVector2f("uOffset", p.position);
            m_shader.setVector4f("uColor", p.color);
            m_texture.bind();
            glBindVertexArray(m_vao);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

unsigned int ParticleGenerator::firstUnusedParticle()
{

    for(unsigned int i{ m_lastUsedParticle }; i < m_amount; ++i)
    {
        if(m_particles.at(i).life <= 0.f)
        {
            m_lastUsedParticle = i;
            return i;
        }
    }

    for(unsigned int i{ 0 }; i < m_lastUsedParticle; ++i)
    {
        if(m_particles.at(i).life <= 0.f)
        {
            m_lastUsedParticle = i;
            return i;
        }
    }

    m_lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, const GameObject& object, const glm::vec2& offset)
{
    float random{ ((rand() % 100) - 50) / 10.f };
    float rColor{ 0.5f + ((rand() % 100) / 100.f) };

    particle.position = object.getPosition() + random + offset;
    particle.color = glm::vec4(rColor, rColor, rColor, 1.f);
    particle.life = 1.f;
    particle.velocity = object.getVelocity() * 0.1f;
}

