cmake_minimum_required(VERSION 3.20)
project(abyss)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW QUIET)
if(NOT GLEW_FOUND)
    find_library(GLEW_LIBRARY NAMES GLEW glew)
    find_path(GLEW_INCLUDE_DIR NAMES GL/glew.h)
    if(GLEW_LIBRARY AND GLEW_INCLUDE_DIR)
        message(STATUS "Found GLEW: ${GLEW_LIBRARY}")
    else()
        message(FATAL_ERROR "Could not find GLEW library")
    endif()
endif()

# Add include directories
include_directories(include)

# Add source files
set(SOURCES
    src/main.cpp
    src/tile.cpp
    src/entity.cpp
    src/level.cpp
)

# Create executable
add_executable(abyss ${SOURCES})

# Link libraries
target_link_libraries(abyss
    OpenGL::OpenGL
    glfw
    ${GLEW_LIBRARY}
)

if(GLEW_INCLUDE_DIR)
    target_include_directories(abyss PRIVATE ${GLEW_INCLUDE_DIR})
endif()

# Copy assets to build directory
add_custom_command(TARGET abyss POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
    COMMENT "Copying assets to build directory"
)

# Set compiler flags for different configurations
if(MSVC)
    target_compile_options(abyss PRIVATE /W4 /WX)
else()
    target_compile_options(abyss PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

