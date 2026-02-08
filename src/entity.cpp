#include "../include/abyss.h"

Entity *get_entity_at(Level &level, int x, int y)
{
    for (auto &entity : level.entities)
    {
        if (entity.x == x && entity.y == y)
        {
            return &entity;
        }
    }
    return nullptr;
}

bool create_player_entity(Level &level)
{
    // Find a valid floor tile to place the player on
    for (int yy = 0; yy < level.height; ++yy) {
        for (int xx = 0; xx < level.width; ++xx) {
            Tile &tile = get_tile(level, xx, yy);
            if (is_walkable(tile)) {
                Entity player;
                player.x = xx;
                player.y = yy;
                player.type = EntityType::PLAYER;
                player.name = "Player";
                player.health = 100;
                level.entities.push_back(player);
                return true;
            }
        }
    }
    return false;
}