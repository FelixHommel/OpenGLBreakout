#include "BallObject.hpp"

BallObject::BallObject()
    : m_radius{ 1.f }
    , m_isStuck{ true }
{}

BallObject::BallObject(const glm::vec2& position, float radius, const glm::vec2& velocity, const Texture2D& texture)
    : GameObject(position, glm::vec2(radius * 2.f), texture, glm::vec3(1.f), velocity)
    , m_radius{ radius }
    , m_isStuck{ true}
{}

glm::vec2 BallObject::move(float dt, unsigned int windowWidth)
{
    if(!m_isStuck)
    {
        m_position += m_velocity * dt;

        if(m_position.x <= 0.f)
        {
            m_velocity.x = -m_velocity.x;
            m_position.x = 0.f;
        }
        else if(m_position.x + m_size.x >= windowWidth)
        {
            m_velocity.x = -m_velocity.x;
            m_position.x = windowWidth = m_size.x;
        }

        if(m_position.y <= 0.f)
        {
            m_velocity.y = -m_velocity.y;
            m_position.y = 0.f;
        }
    }

    return m_position;
}

void BallObject::reset(const glm::vec2& position, const glm::vec2& velocity)
{
    m_position = position;
    m_velocity = velocity;
    m_isStuck = true;
}

Collision BallObject::checkCollisionCircleAABB(const BallObject &first, const GameObject &second)
{
    glm::vec2 center(first.getPosition() + first.getRadius());

    glm::vec2 aabbHalfExtents(second.getSize().x / 2.f, second.getSize().y / 2.f);
    glm::vec2 aabbCenter(second.getPosition().x + aabbHalfExtents.x, second.getPosition().y + aabbHalfExtents.y);

    glm::vec2 difference{ center - aabbCenter };
    glm::vec2 clamped{ glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents) };

    glm::vec2 closest{ aabbCenter + clamped };
    difference = closest - center;
    
    if(glm::length(difference) <= first.getRadius())
        return std::make_tuple(true, vectorDirection(difference), difference);
    else
        return std::make_tuple(false, Direction::UP, glm::vec2(0.f, 0.f));
}

Direction BallObject::vectorDirection(const glm::vec2& target)
{
    std::array<glm::vec2, 4> directions
    {
        glm::vec2(0.f, 1.f),
        glm::vec2(1.f, 0.f),
        glm::vec2(0.f, -1.f),
        glm::vec2(-1.f, 0.f)
    };

    float max{ 0.f };
    int bestMatach{ -1 };

    for(size_t i{ 0 }; i < directions.size(); ++i)
    {
        float dotProduct{ glm::dot(glm::normalize(target), directions.at(i)) };

        if(dotProduct > max)
        {
            max = dotProduct;
            bestMatach = i;
        }
    }

    return static_cast<Direction>(bestMatach);
}
