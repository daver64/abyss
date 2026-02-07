#include "../include/abyss.h"



int main()
{
    if (!init_platform(800, 600, "Abyss"))
    {
        return -1;
    }
    if (!load_texture_atlas("assets/tiles.png", tile_atlas))
    {
        shutdown_platform();
        return -1;
    }
    Level current_level;
    create_level(current_level, 20, 20, LevelType::DUNGEON);
    create_player_entity(current_level, 1, 1);

    while (!want_quit_platform())
    {
        if (!update_platform())
        {
            break;
        }
    }
    unload_texture_atlas(tile_atlas);
    shutdown_platform();
    return 0;
}
