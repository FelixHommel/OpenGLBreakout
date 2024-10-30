#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "glad/include/glad/gl.h"
#include "glm/glm.hpp"

#include "../utility/Texture.hpp"
#include "../utility/SpriteRenderer.hpp"

class GameObject
{
    public:
        GameObject();
        GameObject(const glm::vec2& position, const glm::vec2& size, const Texture2D& texture,
                const glm::vec3& color = glm::vec3(1.f), const glm::vec2& velocity = glm::vec2(0.f));
        virtual ~GameObject() = default;

        virtual void draw(SpriteRenderer& sprite);
        static bool checkCollisionAABB(const GameObject& first, const GameObject& second);

        [[nodiscard]] inline glm::vec2 getPosition() const { return m_position; }
        [[nodiscard]] inline glm::vec2 getSize() const { return m_size; }
        [[nodiscard]] inline glm::vec2 getVelocity() const { return m_velocity; }
        [[nodiscard]] inline bool isDestroyed() const { return m_isDestroyed; }
        [[nodiscard]] inline bool isSolid() const { return m_isSolid; }

        inline void moveX(float offsetX) { m_position.x += offsetX; }
        inline void moveY(float offsetY) { m_position.y += offsetY; }
        inline void inverseVelocityX() { m_velocity.x = -m_velocity.x; }
        inline void inverseVelocityY() { m_velocity.y = -m_velocity.y; }
    
        inline void setVelocityX(float value) { m_velocity.x = value; }
        inline void setVelocityY(float value) { m_velocity.y = value; }
        inline void setVelocity(const glm::vec2& value) { m_velocity = value; }

        inline void setSize(const glm::vec2& value) { m_size = value; }
        
        inline void setPosition(const glm::vec2& value) { m_position = value; }

        inline void setIsSolid() { m_isSolid = true; }
        inline void setDestroyed() { m_isDestroyed = true; }

    protected:
        glm::vec2 m_position;
        glm::vec2 m_size;
        glm::vec2 m_velocity;
        glm::vec3 m_color;

        float m_rotation;

        bool m_isSolid;
        bool m_isDestroyed;

        Texture2D m_texture;
};

#endif //!GAMEOBJECT_HPP
