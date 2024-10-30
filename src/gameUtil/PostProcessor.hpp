#ifndef POSTPROCESSOR_HPP
#define POSTPROCESSOR_HPP

#include "glad/include/glad/gl.h"
#include "glm/glm.hpp"

#include "../utility/Texture.hpp"
#include "../utility/SpriteRenderer.hpp"
#include "../utility/Shader.hpp"

#include <vector>
#include <array>

const std::vector<float> verticesPP = {
    // pos        // tex
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,

    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f,  1.0f, 1.0f, 1.0f
};

constexpr float offset{ 1.f / 300.f };
const float offsets[9][2] = {
    { -offset,  offset  },
    {  0.0f,    offset  },
    {  offset,  offset  },
    { -offset,  0.0f    },
    {  0.0f,    0.0f    },
    {  offset,  0.0f    },
    { -offset, -offset  },
    {  0.0f,   -offset  },
    {  offset, -offset  } 
};

const std::array<int, 9> edgeKernel
{
    -1, -1, -1,
    -1,  8, -1,
    -1, -1, -1
};

const std::array<float, 9> blurKernel
{
    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
    2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
};

class PostProcessor
{
    public:
        PostProcessor(const Shader& shader, unsigned int width, unsigned int height);
        ~PostProcessor();

        inline void activateConfuse() { m_confuse = true; }
        inline void activateChaos() { m_chaos = true; }
        inline void activateShake() { m_shake = true; }

        inline void deactivateConfuse() { m_confuse = false; }
        inline void deactivateChaos() { m_chaos = false; }
        inline void deactivateShake() { m_shake = false; }

        void beginRender() const;
        void endRender() const;

        void render(float time);

    private:
        unsigned int m_msfbo; // msfbo = multisampled fbo
        unsigned int m_fbo;
        unsigned int m_rbo;
        unsigned int m_vao;

        Shader m_ppShader;
        Texture2D m_texture;

        unsigned int m_width;
        unsigned int m_height;

        bool m_confuse;
        bool m_chaos;
        bool m_shake;

        void initRenderData();
};

#endif //!POSTPROCESSOR_HPP
