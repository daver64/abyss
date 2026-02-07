#include "../include/abyss.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <array>

bool want_quit = false;

bool want_quit_platform()
{
    return want_quit;
}
namespace
{
    struct InputState
    {
        std::array<unsigned char, SDL_NUM_SCANCODES> keys{};
        int mouse_x = 0;
        int mouse_y = 0;
        int mouse_wheel = 0;
        std::array<unsigned char, 8> mouse_buttons{};
    };

    SDL_Window *g_window = nullptr;
    SDL_GLContext g_gl_context = nullptr;
    InputState g_input_state;
} // namespace

static bool init_sdl_gl(int width, int height, const std::string &title)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    g_window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
    if (!g_window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        shutdown_platform();
        return false;
    }

    g_gl_context = SDL_GL_CreateContext(g_window);
    if (!g_gl_context)
    {
        SDL_Log("SDL_GL_CreateContext failed: %s", SDL_GetError());
        shutdown_platform();
        return false;
    }

    if (SDL_GL_MakeCurrent(g_window, g_gl_context) != 0)
    {
        SDL_Log("SDL_GL_MakeCurrent failed: %s", SDL_GetError());
        shutdown_platform();
        return false;
    }

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    const GLenum glew_error = glewInit();
    if (glew_error != GLEW_OK)
    {
        SDL_Log("GLEW init failed: %s", glewGetErrorString(glew_error));
        shutdown_platform();
        return false;
    }

    while (glGetError() != GL_NO_ERROR)
    {
    }

    int drawable_width = 0;
    int drawable_height = 0;
    SDL_GL_GetDrawableSize(g_window, &drawable_width, &drawable_height);
    glViewport(0, 0, drawable_width, drawable_height);
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    return true;
}

bool init_platform(int width, int height, const std::string &title)
{
    return init_sdl_gl(width, height, title);
}

bool shutdown_platform()
{
    if (g_gl_context)
    {
        SDL_GL_DeleteContext(g_gl_context);
        g_gl_context = nullptr;
    }

    if (g_window)
    {
        SDL_DestroyWindow(g_window);
        g_window = nullptr;
    }

    SDL_Quit();
    return true;
}

static void handle_event(const SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            const int new_width = event.window.data1;
            const int new_height = event.window.data2;
            glViewport(0, 0, new_width, new_height);
        }
        break;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        g_input_state.keys[event.key.keysym.scancode] =
            static_cast<unsigned char>(event.type == SDL_KEYDOWN);
        break;
    case SDL_MOUSEMOTION:
        g_input_state.mouse_x = event.motion.x;
        g_input_state.mouse_y = event.motion.y;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        if (event.button.button < g_input_state.mouse_buttons.size())
        {
            g_input_state.mouse_buttons[event.button.button] =
                static_cast<unsigned char>(event.type == SDL_MOUSEBUTTONDOWN);
        }
        break;
    case SDL_MOUSEWHEEL:
        g_input_state.mouse_wheel = event.wheel.y;
        break;
    default:
        break;
    }
}

bool update_platform()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
        handle_event(event);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(g_window);
    return true;
}