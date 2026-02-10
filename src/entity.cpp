#include "../include/abyss.h"

#include <cstdlib>

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
                player.tile_index = 39;
                level.entities.push_back(player);
                return true;
            }
        }
    }
    return false;
}

bool create_enemy_entities(Level &level)
{
    static const char *names[] = {"Militia Grunt", "Security Trooper", "Recon Scout"};
    static const int healths[] = {20, 30, 15};
    static const int tiles[] = {45, 46, 47};
    static const int name_count = 3;

    int target_count = 3 + (rand() % 3); // 3-5 enemies
    int spawned = 0;
    int attempts = 0;
    int max_attempts = level.width * level.height * 4;

    while (spawned < target_count && attempts < max_attempts) {
        ++attempts;
        int x = rand() % level.width;
        int y = rand() % level.height;
        Tile &tile = get_tile(level, x, y);
        if (!is_walkable(tile)) {
            continue;
        }
        if (get_entity_at(level, x, y)) {
            continue;
        }

        int pick = rand() % name_count;
        Entity enemy;
        enemy.x = x;
        enemy.y = y;
        enemy.type = EntityType::MONSTER;
        enemy.name = names[pick];
        enemy.health = healths[pick];
        enemy.tile_index = tiles[pick];
        level.entities.push_back(enemy);
        ++spawned;
    }

    return spawned > 0;
}
bool move_entity(Entity &entity, Level &level, int new_x, int new_y)
{
    if (can_move_to(entity, level, new_x, new_y)) {
        entity.x = new_x;
        entity.y = new_y;
        return true;
    }
    return false;
}
bool can_move_to(Entity &entity, Level &level, int new_x, int new_y)
{
    // Check bounds
    if (new_x < 0 || new_x >= level.width || new_y < 0 || new_y >= level.height) {
        return false;
    }
    
    // Check tile walkability
    Tile &tile = get_tile(level, new_x, new_y);
    if (!is_walkable(tile)) {
        return false;
    }
    
    // Check for blocking entities
    for (const auto &other : level.entities) {
        if (other.x == new_x && other.y == new_y && other.type != EntityType::PLAYER) {
            return false; // Can't move onto another entity (except player)
        }
    }
    
    return true;
}

Entity *get_player_entity()
{
    if (g_current_level && !g_current_level->entities.empty()) {
        // Assuming player is always the first entity
        return &g_current_level->entities[0];
    }
    return nullptr;
}