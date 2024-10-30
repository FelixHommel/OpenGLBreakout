#include "SpriteRenderer.hpp"

SpriteRenderer::SpriteRenderer(const Shader& shader)
    : m_quadVAO{ 0 }
    , m_shader{ shader }
{
    unsigned int vbo{};

    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindVertexArray(m_quadVAO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &m_quadVAO);
}

void SpriteRenderer::draw(const Texture2D& texture, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec3& color)
{
    m_shader.use();

    /*  Transformation Order:
     *      1. translation
     *      2. rotation
     *      3. scaling
     */

    glm::mat4 model{ glm::mat4(1.f) };
    model = glm::translate(model, glm::vec3(position, 0.f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f)); //Center the origin of the quad
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f)); //Move origin back

    model = glm::scale(model, glm::vec3(size, 1.f));

    m_shader.setMatrix4("uModel", model);
    m_shader.setVector3f("uColor", color);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
