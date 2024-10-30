#ifndef SPRITERENDERER_HPP
#define SPRITERENDERER_HPP

#include "glad/include/glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include <vector>

const std::vector<float> vertices
{
    //Pos     //Tex
    0.f, 1.f, 0.f, 1.f,
    1.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 0.f,

    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 0.f, 1.f, 0.f
};

constexpr float vertexes[] =
{
    //Pos     //Tex
    0.f, 1.f, 0.f, 1.f,
    1.f, 0.f, 1.f, 0.f,
    0.f, 0.f, 0.f, 0.f,

    0.f, 1.f, 0.f, 1.f,
    1.f, 1.f, 1.f, 1.f,
    1.f, 0.f, 1.f, 0.f
};

class SpriteRenderer
{
    public:
        SpriteRenderer(const Shader& shader);
        ~SpriteRenderer();

        void draw(const Texture2D& texture, const glm::vec2& position, 
                const glm::vec2& size = glm::vec2(10.f, 10.f), float rotation = 0.f, const glm::vec3& color = glm::vec3(1.f));

    private:
        unsigned int m_quadVAO;
        Shader m_shader;
};

#endif //!SPRITERENDERE f_HPP
