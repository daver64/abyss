cmake_minimum_required(VERSION 3.20)
project(abyss)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Find required packages
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
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
    src/platform.cpp
    src/texture_atlas.cpp
    src/draw.cpp
)

# Create executable
add_executable(abyss ${SOURCES})

# Link libraries
target_link_libraries(abyss PRIVATE
    OpenGL::OpenGL
)

if(TARGET SDL2::SDL2)
    target_link_libraries(abyss PRIVATE SDL2::SDL2)
elseif(TARGET SDL2::SDL2main)
    target_link_libraries(abyss PRIVATE SDL2::SDL2 SDL2::SDL2main)
else()
    target_include_directories(abyss PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(abyss PRIVATE ${SDL2_LIBRARIES})
endif()

if(TARGET SDL2_image::SDL2_image)
    target_link_libraries(abyss PRIVATE SDL2_image::SDL2_image)
else()
    target_include_directories(abyss PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
    target_link_libraries(abyss PRIVATE ${SDL2_IMAGE_LIBRARIES})
endif()

if(TARGET GLEW::GLEW)
    target_link_libraries(abyss PRIVATE GLEW::GLEW)
elseif(GLEW_LIBRARY)
    target_link_libraries(abyss PRIVATE ${GLEW_LIBRARY})
endif()

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
    target_compile_options(abyss PRIVATE -Wall  -Werror)
endif()

