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

set(IRRKLANG_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/irrKlang/include")
set(IRRKLANG_LIB_DIR
    "${CMAKE_SOURCE_DIR}/dependencies/irrKlang/bin/linux-gcc-64"
    "${CMAKE_SOURCE_DIR}/dependencies/irrKlang/lib/Winx64-visualStudio")

if(UNIX)
    find_path(
        IRRKLANG_INCLUDE_DIR irrKlang.h PATHS
        ${IRRKLANG_INCLUDE_DIR} 
        DOC "Path to irrKlang headers on Linux"
    )
    find_library(
        IRRKLANG_LIBRARY NAMES irrKlang.so PATHS
        ${IRRKLANG_LIB_DIR}
        DOC "Path to irrKlang library on Linux"
    )
elseif(WIN32)
    find_path(
        IRRKLANG_INCLUDE_DIR irrKlang.h PATHS
        ${IRRKLANG_INCLUDE_DIR} 
        DOC "Path to irrKlang headers on Linux"
    )
    find_library(
        IRRKLANG_LIBRARY NAMES irrKlang.lib PATHS
        ${IRRKLANG_LIB_DIR}
        DOC "Path to irrKlang library on Linux"
    )
endif()

#Verify that there was some file found to link
if (NOT IRRKLANG_INCLUDE_DIR OR NOT IRRKLANG_LIBRARY)
    message(FATAL_ERROR "irrKlang not found. Make sure the headers and libraries are installed in a known location.")
endif()

add_executable(${PROJECT_NAME}
    dependencies/glad/src/gl.c
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
add_subdirectory(dependencies/freetype)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    ${IRRKLANG_LIBRARY}
    freetype
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
