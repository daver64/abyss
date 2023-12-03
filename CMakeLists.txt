
##
## Abyss Project Details
##
## (c) 2023 David Rowbotham (thedaver64@gmail.com)
##
##
cmake_minimum_required(VERSION 3.0...3.22)
project("abyss")
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "64 bit compiler required.")
endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")



##
## Linux Package Requirements
##
if(UNIX)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories($OPENGL_INCLUDE_DIR src/imgui)
endif(UNIX)


##
## MSVC Compile Flags
##
if(MSVC)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-I../include -I../src/imgui -I../src/imgui/backends /EHsc /Qpar /GS- /O2 /D NDEBUG")
endif(MSVC)


##
## Linux Compile Flags
##
if(UNIX)
set(CMAKE_CXX_FLAGS "-I../include -I../src/imgui -I../src/imgui/backends -O2")
endif(UNIX)


##
## MINGW. Not supported atm.
##
if(MINGW)
message(FATAL_ERROR "MinGW not currently supported.")
endif(MINGW)


##
## Project Build.
##
if(MSVC)
add_executable(abyss
    src/glext.cpp
    src/main.cpp
    src/fastnoise.cpp
    src/texture.cpp
    src/screen.cpp
    src/drawbuffer.cpp
    src/maths.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/backends/imgui_impl_glfw.cpp
    src/imgui/backends/imgui_impl_opengl3.cpp
    src/imgui/backends/imgui_impl_win32.cpp
    src/abyss.rc)
endif(MSVC)

if(UNIX)
add_executable(abyss
    src/main.cpp
    src/fastnoise.cpp
    src/texture.cpp
    src/screen.cpp
    src/drawbuffer.cpp
    src/maths.cpp
    src/imgui/imgui.cpp
    src/imgui/imgui_draw.cpp
    src/imgui/imgui_tables.cpp
    src/imgui/imgui_widgets.cpp
    src/imgui/backends/imgui_impl_glfw.cpp
    src/imgui/backends/imgui_impl_opengl3.cpp)
endif(UNIX)


##
## MSVC Platform Link Options
##
if(MSVC)
set_target_properties(abyss PROPERTIES LINK_FLAGS "/LIBPATH:../lib/win64 /ENTRY:mainCRTStartup /SUBSYSTEM:CONSOLE")
target_link_libraries(abyss glfw3 opengl32 glu32 openal32)
endif(MSVC)

##
## Linux Platform Link Options
##
if(UNIX)
target_link_libraries(abyss GL GLU glfw openal)
endif(UNIX)