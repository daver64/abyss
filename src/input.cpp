#include "../include/abyss.h"

#include <GL/glew.h>
#include <SDL2/SDL.h>


bool keydown_callback(int scancode)
{


    switch(scancode)
    {
        case SDL_SCANCODE_ESCAPE:
            quit_platform();
            return false;
            
        default:
            break;
    }
    return true;
}