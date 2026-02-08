#include "../include/abyss.h"

Level *g_current_level = nullptr;

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
    if(!load_font("", 16))
    {
        unload_texture_atlas(tile_atlas);
        shutdown_platform();
        return -1;
    }
    Level current_level;
    create_level(current_level, 20, 10, LevelType::DUNGEON);
    create_player_entity(current_level);
    g_current_level = &current_level;

    while (!want_quit_platform())
    {
        clear_screen();
        update_platform();
        draw_level(current_level,2.0f);
        gprintf(10, 600, {255,255,255,255}, "Player HP: %d", current_level.entities[0].health);
        swap_buffers();
    }
    unload_font();
    unload_texture_atlas(tile_atlas);
    shutdown_platform();
    return 0;
}
