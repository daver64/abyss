#include "../include/abyss.h"



int main()
{
    if (!init_platform(1024, 768, "Abyss"))
    {
        return -1;
    }
    if (!load_texture_atlas("assets/textures/tiles.png", tile_atlas))
    {
        shutdown_platform();
        return -1;
    }
    Level current_level;
    create_level(current_level, 20, 10, LevelType::DUNGEON);
    create_player_entity(current_level);

    while (!want_quit_platform())
    {
        clear_screen();
        update_platform();
        draw_level(current_level,2.0f);
        swap_buffers();
    }
    unload_texture_atlas(tile_atlas);
    shutdown_platform();
    return 0;
}
