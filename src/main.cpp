#include "glad/include/glad/gl.h"
#include <GLFW/glfw3.h>

#include "Game.hpp"
#include "utility/ResourceManager.hpp"

#include <iostream>
#include <stdexcept>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

constexpr unsigned int SCREEN_WIDTH{ 800 };
constexpr unsigned int SCREEN_HEIGHT{ 600 };

Game breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char** argv)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if(!gladLoadGL(glfwGetProcAddress))
    {
        throw std::runtime_error("Could not load GLAD");
        return 1;
    }

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    breakout.init();

    float deltatime{ 0.f };
    float lastFrame{ 0.f };

    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltatime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        breakout.processInput(deltatime);
        breakout.update(deltatime);

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        breakout.render();

        glfwSwapBuffers(window);
    }

    ResourceManager::clear();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(key >= 0 && key < 1024)
    {
        if(action == GLFW_PRESS)
            breakout.setKeyPressed(key);
        else if(action == GLFW_RELEASE)
            breakout.setKeyReleased(key);
    }
}
