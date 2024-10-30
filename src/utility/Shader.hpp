#ifndef SHADER_HPP
#define SHADER_HPP

#include "glad/include/glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <iostream>

class Shader
{
    public:
        Shader() = default;
        ~Shader() = default;

        Shader& use();
        [[nodiscard]] inline unsigned int ID() const { return m_ID; }

        //Gemoetry shader is optional
        void compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource = nullptr);

        void setFloat(const std::string& name, float value, bool useShader = false);
        void setInt(const std::string& name, int value, bool useShader = false);
        void setVector2f(const std::string& name, float x, float y, bool useShader = false);
        void setVector2f(const std::string& name, const glm::vec2& value, bool useShader = false);
        void setVector3f(const std::string& name, float x, float y, float z, bool useShader = false);
        void setVector3f(const std::string& name, const glm::vec3& value, bool useShader = false);
        void setVector4f(const std::string& name, float x, float y, float z, float w, bool useShader = false);
        void setVector4f(const std::string& name, const glm::vec4& value, bool useShader = false);
        void setMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader = false);

    private:
        unsigned int m_ID;

        void checkCompileError(unsigned int object, unsigned int type);
};

#endif //!SHADER_HPP
