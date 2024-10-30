#ifndef PARTICLEGENERATOR_HPP
#define PARTICLEGENERATOR_HPP

#include "glad/include/glad/gl.h"
#include "glm/glm.hpp"

#include "../utility/Shader.hpp"
#include "../utility/Texture.hpp"
#include "GameObject.hpp"

#include "vector"
#include <cstdlib>

const std::vector<float> particleVertices
{
    0.f, 1.f, 0.f, 1.f,
    1.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 0.f,

    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 0.f, 1.f, 0.f
};

struct Particle
{
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float life{ 0.f };

    Particle()
        : position{ 0.f }
        , velocity{ 0.f }
        , color{ 1.f }
    {}
};

class ParticleGenerator
{
    public:
        ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount);

        void update(float dt, const GameObject& object, unsigned int newParticles, const glm::vec2& offset = glm::vec2(0.f));
        void draw();

    private:
        std::vector<Particle> m_particles;
        unsigned int m_amount;
        unsigned int m_lastUsedParticle;

        Shader m_shader;
        Texture2D m_texture;
        unsigned int m_vao;

        unsigned int firstUnusedParticle();
        void respawnParticle(Particle& particle, const GameObject& object, const glm::vec2& offset = glm::vec2(0.f));
};

#endif //!PARTICLEGENERATOR_HPP
