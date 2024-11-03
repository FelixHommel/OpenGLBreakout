#include "Game.hpp"
#include "freetype/freetype.h"
#include "imgui/imgui.h"

Game::Game(unsigned int windowWidth, unsigned int windowHeight)
    : m_state{ GameState::GAME_ACTIVE }
    , m_keys{}
    , m_windowWidth{ windowWidth }
    , m_windowHeight{ windowHeight }
    , m_renderer{ nullptr }
    , m_player{ nullptr }
    , m_ball{ nullptr }
    , m_particles{ nullptr }
    , m_postProcessor{ nullptr }
    , m_soundEngine{ irrklang::createIrrKlangDevice() }
    , m_ft{}
    , m_currentLevel{ 0 }
    , m_shakeTimer{ 0.f }
{}

Game::~Game()
{
    delete m_soundEngine;
    delete m_postProcessor;
    delete m_particles;
    delete m_ball;
    delete m_player;
    delete m_renderer;
}

void Game::init()
{
    ResourceManager::loadShader("resources/shaders/basic.vert", "resources/shaders/basic.frag", nullptr, "sprite"); 

    glm::mat4 projection{ glm::ortho(0.f, static_cast<float>(m_windowWidth), static_cast<float>(m_windowHeight), 0.f, -1.f, 1.f) };

    ResourceManager::getShader("sprite").use().setInt("uTexture", 0);
    ResourceManager::getShader("sprite").setMatrix4("uProjection", projection);

    m_renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    ResourceManager::loadTexture("resources/textures/block.png", false, "block");
    ResourceManager::loadTexture("resources/textures/block_solid.png", false, "block_solid");
    ResourceManager::loadTexture("resources/textures/background.jpg", false, "background");
    ResourceManager::loadTexture("resources/textures/awesomeface.png", true, "face");
    ResourceManager::loadTexture("resources/textures/paddle.png", true, "paddle");

    m_levels.emplace_back("resources/levels/standard.lvl", m_windowWidth, m_windowHeight / 2);
    m_levels.emplace_back("resources/levels/fewSmallGaps.lvl", m_windowWidth, m_windowHeight / 2);
    m_levels.emplace_back("resources/levels/spaceInvaders.lvl", m_windowWidth, m_windowHeight / 2);
    m_levels.emplace_back("resources/levels/bounceGalore.lvl", m_windowWidth, m_windowHeight / 2);
    m_currentLevel = 0;

    glm::vec2 playerPos(m_windowWidth / 2.f - playerSize.x / 2.f, m_windowHeight - playerSize.y);
    m_player = new GameObject(playerPos, playerSize, ResourceManager::getTexture("paddle"));

    glm::vec2 ballPos{ playerPos + glm::vec2(playerSize.x / 2.f - ballRadius, -ballRadius * 2.f) };
    m_ball = new BallObject(ballPos, ballRadius, initialBallVelocity, ResourceManager::getTexture("face"));

    ResourceManager::loadTexture("resources/textures/particle.png", true, "particle");
    ResourceManager::loadShader("resources/shaders/particles.vert", "resources/shaders/particles.frag", nullptr, "particleShader");
    ResourceManager::getShader("particleShader").use().setInt("uTexture", 0);
    ResourceManager::getShader("particleShader").setMatrix4("uProjection", projection);

    m_particles = new ParticleGenerator(ResourceManager::getShader("particleShader"), ResourceManager::getTexture("particle"), 500);

    m_soundEngine->play2D("resources/audio/breakout.mp3", true);

    ResourceManager::loadShader("resources/shaders/postProcessing.vert", "resources/shaders/postProcessing.frag", nullptr, "postProcessing");
    m_postProcessor = new PostProcessor(ResourceManager::getShader("postProcessing"), m_windowWidth, m_windowHeight);

    if(FT_Init_FreeType(&m_ft))
    {
        std::cerr << "Could not initialize freetype" << std::endl;
        std::exit(1);
    }
}

void Game::processInput(float dt)
{
    if(m_state == GameState::GAME_ACTIVE)
    {
        float velocity = playerVelocity * dt;

        if(m_keys.at(GLFW_KEY_A))
        {
            if(m_player->getPosition().x >= 0.f)
            {
                m_player->moveX(-velocity);
                if(m_ball->isStuck())
                    m_ball->moveX(-velocity);
            }
        }
        else if(m_keys.at(GLFW_KEY_D))
        {
            if(m_player->getPosition().x <= m_windowWidth - m_player->getSize().x)
            {
                m_player->moveX(velocity);
                if(m_ball->isStuck())
                    m_ball->moveX(velocity);
            }
        }

        if(m_keys.at(GLFW_KEY_SPACE))
            m_ball->setUnstuck();
    }
}

void Game::update(float dt)
{
    m_ball->move(dt, m_windowWidth);
    checkCollisions();

    m_particles->update(dt, *m_ball, 2, glm::vec2(m_ball->getRadius() / 2.f));
    
    if(m_ball->getPosition().y >= m_windowHeight)
    {
        resetLevel();
        resetPlayer();
    }

    if(m_shakeTimer > 0.f)
    {
        m_shakeTimer -= dt;
        if(m_shakeTimer <= 0.f)
            m_postProcessor->deactivateShake();
    }
}

void Game::render()
{
    if(m_state == GameState::GAME_ACTIVE)
    {
        m_postProcessor->beginRender();
        m_renderer->draw(ResourceManager::getTexture("background"), glm::vec2(0.f, 0.f), glm::vec2(m_windowWidth, m_windowHeight), 0.f);
        m_levels.at(m_currentLevel).draw(*m_renderer);
        m_player->draw(*m_renderer);
        m_particles->draw();
        m_ball->draw(*m_renderer);
        m_postProcessor->endRender();
        m_postProcessor->render(glfwGetTime());
    }
}

void Game::resetLevel()
{
    switch(m_currentLevel)
    {
        case 0: 
            m_levels.at(0).load(levelMapping.at(0).c_str(), m_windowWidth, m_windowHeight / 2.f);
            break;
        case 1:
            m_levels.at(1).load(levelMapping.at(1).c_str(), m_windowWidth, m_windowHeight / 2.f);
            break;
        case 2:
            m_levels.at(2).load(levelMapping.at(2).c_str(), m_windowWidth, m_windowHeight / 2.f);
            break;
        case 3:
            m_levels.at(3).load(levelMapping.at(3).c_str(), m_windowWidth, m_windowHeight / 2.f);
            break;
        default:
            throw std::runtime_error("Game::resetLevel(): m_currentLevel is in an invalid state");
    }
}

void Game::resetPlayer()
{
    m_player->setSize(playerSize);
    m_player->setPosition(glm::vec2(m_windowWidth / 2.f - playerSize.x / 2.f, m_windowHeight - playerSize.y));
    m_ball->reset(m_player->getPosition() + glm::vec2(playerSize.x / 2.f - ballRadius, -(ballRadius * 2.f)), initialBallVelocity);
}

void Game::checkCollisions()
{
    for(auto& box : m_levels.at(m_currentLevel).getBricksModifiable())
    {
        if(!box.isDestroyed())
        {
            Collision collision{ BallObject::checkCollisionCircleAABB(*m_ball, box) };

            if(std::get<0>(collision))
            {
                if(!box.isSolid())
                {
                    box.setDestroyed();
                    m_soundEngine->play2D("resources/audio/bleep.mp3", false);
                }
                else
                {
                    m_shakeTimer = 0.05f;
                    m_postProcessor->activateShake();
                    m_soundEngine->play2D("resources/audio.solid.wav", false);
                }

                Direction dir{ std::get<1>(collision) };
                glm::vec2 diffVector{ std::get<2>(collision) };

                if(dir == Direction::LEFT || dir == Direction::RIGHT) //Horizontal collision
                {
                    m_ball->inverseVelocityX();

                    float penetration{ m_ball->getRadius() - std::abs(diffVector.x) };

                    if(dir == Direction::LEFT)
                        m_ball->moveX(penetration);
                    else
                        m_ball->moveX(-penetration);
                }
                else //Vertical collision
                {
                    //m_ball->setVelocityY(-1.f * std::abs(m_ball->getVelocity().y));
                    m_ball->inverseVelocityY();

                    float penetration{ m_ball->getRadius() - std::abs(diffVector.y) };

                    if(dir == Direction::UP)
                        m_ball->moveY(-penetration);
                    else
                        m_ball->moveY(penetration);
                }
            }
        }
    }

    Collision result{ BallObject::checkCollisionCircleAABB(*m_ball, *m_player) };
    if(!m_ball->isStuck() && std::get<0>(result))
    {
        float centerBoard{ m_player->getPosition().x + (m_player->getSize().x / 2.f) };
        float distance{ (m_ball->getPosition().x + m_ball->getRadius()) - centerBoard };
        float percentage{ distance / (m_player->getSize().x / 2.f) };

        float strength{ 2.f };
        glm::vec2 oldVelocity{ m_ball->getVelocity() };
        m_ball->setVelocityX(initialBallVelocity.x * percentage * strength);
        m_ball->inverseVelocityY();
        m_ball->setVelocity(glm::normalize(m_ball->getVelocity()) * glm::length(oldVelocity));
    }
}
