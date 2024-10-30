#ifndef BALLOBJECT_HPP
#define BALLOBJECT_HPP

#include "GameObject.hpp"

#include <tuple>

enum class Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

using Collision = std::tuple<bool, Direction, glm::vec2>;

class BallObject : public GameObject
{
    public:
        BallObject();
        BallObject(const glm::vec2& position, float radius, const glm::vec2& velocity, const Texture2D& texture);

        [[nodiscard]] bool isStuck() const { return m_isStuck; }
        [[nodiscard]] float getRadius() const { return m_radius; }

        void setUnstuck() { m_isStuck = false; }
        void setStuck() { m_isStuck = true; }

        glm::vec2 move(float dt, unsigned int windowWidth);
        void reset(const glm::vec2& position, const glm::vec2& velocity);
        static Collision checkCollisionCircleAABB(const BallObject& first, const GameObject& second);
        static Direction vectorDirection(const glm::vec2& target);

    private:
        float m_radius;
        bool m_isStuck;
};

#endif //!BALLOBJECT_HPP
