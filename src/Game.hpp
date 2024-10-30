#ifndef GAME_HPP
#define GAME_HPP

#include "glad/include/glad/gl.h"
#include "glfw/include/GLFW/glfw3.h"
#include <irrKlang.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "utility/SpriteRenderer.hpp"
#include "utility/ResourceManager.hpp"
#include "gameUtil/GameLevel.hpp"
#include "gameUtil/BallObject.hpp"
#include "gameUtil/ParticleGenerator.hpp"
#include "gameUtil/PostProcessor.hpp"

#include <array>
#include <vector>

enum class GameState
{
    GAME_MENU,
    GAME_ACTIVE,
    GAME_WIN
};

const std::map<int, std::string> levelMapping
{
    { 0, "resources/levels/standard.lvl" },
    { 1, "resources/levels/fewSmallGaps.lvl" },
    { 2, "resources/levels/spaceInvaders" },
    { 3, "resources/levels/bounceGalore.lvl" }
};

constexpr glm::vec2 playerSize(100.f, 20.f);
constexpr float playerVelocity{ 500.f };
constexpr glm::vec2 initialBallVelocity(100.f, -350.f);
constexpr float ballRadius{ 12.5f };

class Game
{
    public:
        Game(unsigned int windowWidth, unsigned int windowHeight);
        ~Game();

        void init(GLFWwindow* window);

        void processInput(float dt);
        inline void setKeyPressed(int key) { m_keys.at(key) = true; }
        inline void setKeyReleased(int key) { m_keys.at(key) = false; }

        void update(float dt);
        void render();

    private:
        GameState m_state;
        std::array<bool, 1024> m_keys;
        unsigned int m_windowWidth;
        unsigned int m_windowHeight;

        SpriteRenderer* m_renderer;
        GameObject* m_player;
        BallObject* m_ball;
        ParticleGenerator* m_particles;
        PostProcessor* m_postProcessor;
        irrklang::ISoundEngine* m_soundEngine;

        std::vector<GameLevel> m_levels;
        unsigned int m_currentLevel;

        float m_shakeTimer;

        void resetLevel();
        void resetPlayer();

        void checkCollisions();
        void renderTransparentTextField(const char* label, char* buffer, size_t bufferSize);
};

#endif //!GAME_HPP
