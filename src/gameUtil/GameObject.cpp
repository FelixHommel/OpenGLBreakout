#include "GameObject.hpp"

GameObject::GameObject()
    : m_position{ 0.f, 0.f }
    , m_size{ 1.f, 1.f }
    , m_velocity{ 0.f }
    , m_color{ 1.f }
    , m_rotation{ 0.f }
    , m_isSolid{ false }
    , m_isDestroyed{ false }
{}

GameObject::GameObject(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture, const glm::vec3& color, const glm::vec2& velocity)
    : m_position{ position }
    , m_size{ size }
    , m_velocity{ velocity }
    , m_color{ color }
    , m_rotation{ 0.f }
    , m_isSolid{ false }
    , m_isDestroyed{ false }
    , m_texture{ texture }
{}

void GameObject::draw(SpriteRenderer& sprite)
{
    sprite.draw(m_texture, m_position, m_size, m_rotation, m_color);
}

bool GameObject::checkCollisionAABB(const GameObject &first, const GameObject &second)
{
    bool collisionX{ first.getPosition().x + first.getSize().x >= second.getPosition().x && second.getPosition().x + second.getSize().x >= first.getPosition().x };
    bool collisionY{ first.getPosition().y + first.getSize().y >= second.getPosition().y && second.getPosition().y + second.getSize().y >= first.getPosition().y };

    return collisionX && collisionY;
}
