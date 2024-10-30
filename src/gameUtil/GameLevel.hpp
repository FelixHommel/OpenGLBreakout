#ifndef GAMELEVEL_HPP
#define GAMELEVEL_HPP

#include "GameObject.hpp"
#include "../utility/ResourceManager.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using mapLayout = std::vector<std::vector<unsigned int>>;

enum TileType
{
    TYPE_SOLID = 1,
    TYPE_WHITE = 2,
    TYPE_BLUE = 3,
    TYPE_GREEN = 4,
    TYPE_YELLOW = 5
};

class GameLevel
{
    public:
        GameLevel() = default;
        GameLevel(const char* filepath, unsigned int levelWidth, unsigned int levelHeight);
        ~GameLevel() = default;

        void load(const char* filepath, unsigned int levelWidth, unsigned int levelHeight);

        void draw(SpriteRenderer& renderer);

        [[nodiscard]] bool isComplete() const;
        [[nodiscard]] inline const std::vector<GameObject>& getBricks() { return m_bricks; }
        [[nodiscard]] inline std::vector<GameObject>& getBricksModifiable() { return m_bricks; }

    private:
        std::vector<GameObject> m_bricks;

        void init(const mapLayout& tileData, unsigned int levelWidth, unsigned int levelHeight);
};

#endif //!GAMELEVEL_HPP
