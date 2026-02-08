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
            
        case SDL_SCANCODE_UP:
            if (g_current_level) {
                Entity *player = get_player_entity();
                if (player) {
                    move_entity(*player, *g_current_level, player->x, player->y - 1);
                }
            }
            break;
            
        case SDL_SCANCODE_DOWN:
            if (g_current_level) {
                Entity *player = get_player_entity();
                if (player) {
                    move_entity(*player, *g_current_level, player->x, player->y + 1);
                }
            }
            break;
            
        case SDL_SCANCODE_LEFT:
            if (g_current_level) {
                Entity *player = get_player_entity();
                if (player) {
                    move_entity(*player, *g_current_level, player->x - 1, player->y);
                }
            }
            break;
            
        case SDL_SCANCODE_RIGHT:
            if (g_current_level) {
                Entity *player = get_player_entity();
                if (player) {
                    move_entity(*player, *g_current_level, player->x + 1, player->y);
                }
            }
            break;
            
        default:
            break;
    }
    return true;
}