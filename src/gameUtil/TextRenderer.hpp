#ifndef TEXTRENDERER_HPP
#define TEXTRENDERER_HPP

#include "glad/include/glad/gl.h"
#include "glm/glm.hpp"

#include "../utility/Texture.hpp"
#include "../utility/Shader.hpp"

struct Character
{
    unsigned int textureID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer
{

};

#endif //!TEXTRENDERER_HPP
