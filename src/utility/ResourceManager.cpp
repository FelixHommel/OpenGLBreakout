#include "ResourceManager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

std::map<std::string, Shader> ResourceManager::s_shaders;
std::map<std::string, Texture2D> ResourceManager::s_textures;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, const std::string& name)
{
    s_shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);

    return s_shaders[name];
}

Texture2D ResourceManager::loadTexture(const char* filepath, bool alpha, const std::string& name)
{
    s_textures[name] = loadTextureFromFile(filepath, alpha);

    return s_textures[name];
}

void ResourceManager::clear()
{
    for(auto& i : s_shaders)
        glDeleteProgram(i.second.ID());
    for(auto& i : s_textures)
        glDeleteTextures(1, &i.second.ID());
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    std::string vertCode;
    std::string fragCode;
    std::string geomCode;

    std::ifstream vertShaderFile;
    std::ifstream fragShaderFile;
    try
    {
        vertShaderFile.open(vShaderFile);
        fragShaderFile.open(fShaderFile);

        std::stringstream vShaderStream;
        std::stringstream fShaderStream;

        vShaderStream << vertShaderFile.rdbuf();
        fShaderStream << fragShaderFile.rdbuf();

        vertShaderFile.close();
        fragShaderFile.close();

        vertCode = vShaderStream.str();
        fragCode = fShaderStream.str();

        if(gShaderFile != nullptr)
        {
            std::ifstream geomShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geomShaderFile.rdbuf();
            geomShaderFile.close();
            geomCode = gShaderStream.str();
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }

    const char* vShaderCode{ vertCode.c_str() };
    const char* fShaderCode{ fragCode.c_str() };
    const char* gShaderCode{ geomCode.c_str() };

    Shader shader{};
    shader.compile(vShaderCode, fShaderCode, gShaderCode != nullptr ? nullptr : gShaderCode);

    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* filepath, bool alpha)
{
    Texture2D texture;

    if(alpha)
    {
       texture.setInternalFormat(GL_RGBA);
       texture.setImageFormat(GL_RGBA);
    }

    int width{};
    int height{};
    int nrChannels{};

    unsigned char* data{ stbi_load(filepath, &width, &height, &nrChannels, 0) };

    if(!data)
        std::cerr << "The image could not be read (make sure the filepath is correct)" << std::endl;

    texture.generate(width, height, data);

    stbi_image_free(data);

    return texture;
}
