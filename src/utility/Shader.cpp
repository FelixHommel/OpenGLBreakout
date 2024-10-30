#include "Shader.hpp"

Shader& Shader::use()
{
    glUseProgram(m_ID);
    return *this;

}

void Shader::compile(const char* vertexSource, const char* fragmentSource, const char* geometrySource)
{
    unsigned int sVert{ glCreateShader(GL_VERTEX_SHADER) };
    glShaderSource(sVert, 1, &vertexSource, nullptr);
    glCompileShader(sVert);
    checkCompileError(sVert, GL_VERTEX_SHADER);

    unsigned int sFrag{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(sFrag, 1, &fragmentSource, nullptr);
    glCompileShader(sFrag);
    checkCompileError(sFrag, GL_FRAGMENT_SHADER);

    unsigned int sGeo{};
    if(geometrySource != nullptr)
    {
        sGeo = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(sGeo, 1, &geometrySource, nullptr);
        glCompileShader(sGeo);
        checkCompileError(sGeo, GL_GEOMETRY_SHADER);
    }

    m_ID = glCreateProgram();

    glAttachShader(m_ID, sVert);
    glAttachShader(m_ID, sFrag);
    if(geometrySource != nullptr)
    {
        std::cout << "here" << std::endl;
        glAttachShader(m_ID, sGeo);
    }

    glLinkProgram(m_ID);
    checkCompileError(m_ID, GL_PROGRAM);

    glDeleteShader(sVert);
    glDeleteShader(sFrag);
    if(geometrySource != nullptr)
        glDeleteShader(sGeo);
}

void Shader::setFloat(const std::string& name, float value, bool useShader)
{
    if(useShader)
        this->use();

    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value, bool useShader)
{
    if(useShader)
        this->use();

    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setVector2f(const std::string& name, float x, float y, bool useShader)
{
    if(useShader)
        this->use();

    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
}

void Shader::setVector2f(const std::string& name, const glm::vec2& value, bool useShader)
{
    if(useShader)
        this->use();

    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVector3f(const std::string& name, float x, float y, float z, bool useShader)
{
    if(useShader)
        this->use();

    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
}

void Shader::setVector3f(const std::string& name, const glm::vec3& value, bool useShader)
{
    if(useShader)
        this->use();

    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVector4f(const std::string& name, float x, float y, float z, float w, bool useShader)
{
    if(useShader)
        this->use();

    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
}

void Shader::setVector4f(const std::string& name, const glm::vec4& value, bool useShader)
{
    if(useShader)
        this->use();

    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader)
{
    if(useShader)
        this->use();

    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileError(unsigned int object, unsigned int type)
{
    int success{};
    char infoLog[1024];

    if(type != GL_PROGRAM)
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Compile-time error: Type:" << type << std::endl
                << infoLog << std::endl << "---------------------------------------------" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER: Link-time error: Type:" << type << std::endl
                << infoLog << std::endl << "---------------------------------------------" << std::endl;
        }
    }
}

