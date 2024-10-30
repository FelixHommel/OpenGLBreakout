#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "glad/include/glad/gl.h"

#include "Shader.hpp"
#include "Texture.hpp"

#include <map>
#include <string>
#include <fstream>
#include <sstream>

class ResourceManager
{
    public:
        ResourceManager() = delete;
        ~ResourceManager() = delete;

        static Shader loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name);
        static Texture2D loadTexture(const char* filepath, bool alpha, const std::string& name);

        static inline Shader getShader(const std::string& name) { return s_shaders.at(name); };
        static inline Texture2D getTexture(const std::string& name) { return s_textures.at(name); };

        static void clear();

        static std::map<std::string, Shader> s_shaders;
        static std::map<std::string, Texture2D> s_textures;

    private:
        static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
        static Texture2D loadTextureFromFile(const char* filepath, bool alpha);
};

#endif //!RESOURCEMANAGER_HPP
