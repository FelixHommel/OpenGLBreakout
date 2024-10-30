#include "GameLevel.hpp"

GameLevel::GameLevel(const char* filepath, unsigned int levelWidth, unsigned int levelHeight)
{
    load(filepath, levelWidth, levelHeight);
}

void GameLevel::load(const char* filepath, unsigned int levelWidth, unsigned int levelHeight)
{
    m_bricks.clear();

    unsigned int tileCode{};
    GameLevel level;
    std::string line;
    std::ifstream fstream(filepath);
    mapLayout tileData;
    if(fstream.is_open())
    {
        while(std::getline(fstream, line))
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while(sstream >> tileCode)
                row.push_back(tileCode);

            tileData.push_back(row);
        }

        if(tileData.size() > 0)
            init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::init(const mapLayout& tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    unsigned int height{ static_cast<unsigned int>(tileData.size()) };
    unsigned int width{ static_cast<unsigned int>(tileData.at(0).size()) };

    float unitWidth{ levelWidth / static_cast<float>(width) };
    float unitHeight{ levelHeight / static_cast<float>(height) };

    for(size_t y{ 0 }; y < height; ++y)
    {
        for(size_t x{ 0 }; x < width; ++x)
        {
            if(tileData[y][x] == TYPE_SOLID)
            {
                glm::vec2 pos(unitWidth * x, unitHeight * y);
                glm::vec2 size(unitWidth, unitHeight);

                GameObject obj(pos, size, ResourceManager::getTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.setIsSolid();

                m_bricks.push_back(obj);
            }
            else if(tileData[y][x] > TYPE_SOLID)
            {
                glm::vec3 color(1.f);

                switch(tileData[y][x])
                {
                    case TYPE_WHITE:
                        color = glm::vec3(0.2f, 0.6f, 1.f);
                        break;
                    case TYPE_BLUE:
                        color = glm::vec3(0.f, 0.7f, 0.f);
                        break;
                    case TYPE_GREEN:
                        color = glm::vec3(0.8f, 0.8f, 0.4f);
                        break;
                    case TYPE_YELLOW:
                        color = glm::vec3(1.f, 0.5f, 0.f);
                        break;
                }

                glm::vec2 pos(unitWidth * x, unitHeight * y);
                glm::vec2 size(unitWidth, unitHeight);

                m_bricks.emplace_back(pos, size, ResourceManager::getTexture("block"), color);
            }
        }
    }
}

void GameLevel::draw(SpriteRenderer& renderer)
{
    for(GameObject& tile : m_bricks)
        if(!tile.isDestroyed())
            tile.draw(renderer);
}

bool GameLevel::isComplete() const
{
    /*
    for(const auto& tile : m_bricks)
        if(!tile.isSolid() && !tile.isDestroyed())
            return false;
    return true;
    */
    
    return std::all_of(m_bricks.begin(), m_bricks.end(), [](const auto& tile) { return tile.isSolid() || tile.isDestroyed(); });
}

