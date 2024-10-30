cmake_minimum_required(VERSION 3.22.1)
project(Breakout VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

set(GLM_ENABLE_CXX_17 ON)
set(GLM_ENABLE_FAST_MATH ON)

set(IRRKLANG_DIR ~/irrKlang)
set(IRRKLANG_INCLUDE_DIR ${IRRKLANG_DIR}/include)
set(IRRKLANG_LIB_DIR ${IRRKLANG_DIR}/bin/linux-gcc-64)

set(IMGUI_DIR dependencies/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)


add_executable(${PROJECT_NAME}
    dependencies/glad/src/gl.c
    ${IMGUI_SOURCES}
    src/main.cpp
    src/Game.cpp
    src/utility/Shader.cpp
    src/utility/Texture.cpp
    src/utility/ResourceManager.cpp
    src/utility/SpriteRenderer.cpp
    src/gameUtil/GameObject.cpp
    src/gameUtil/GameLevel.cpp
    src/gameUtil/BallObject.cpp
    src/gameUtil/ParticleGenerator.cpp
    src/gameUtil/PostProcessor.cpp
)

find_package(OpenGL REQUIRED)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    ${IRRKLANG_LIB_DIR}/libIrrKlang.so
)

target_sources(${PROJECT_NAME} PRIVATE
    #Headers and inline files go here
    src/Game.hpp
    src/utility/Shader.hpp
    src/utility/Texture.hpp
    src/utility/ResourceManager.hpp
    src/utility/SpriteRenderer.hpp
    src/gameUtil/GameObject.hpp
    src/gameUtil/GameLevel.hpp
    src/gameUtil/BallObject.hpp
    src/gameUtil/ParticleGenerator.hpp
    src/gameUtil/PostProcessor.hpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
    dependencies 
    ${IRRKLANG_INCLUDE_DIR}
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    GLFW_INCLUDE_NONE
)
