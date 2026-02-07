#include "../include/abyss.h"

Entity *get_entity_at(Level &level, int x, int y) {
    for (auto &entity : level.entities) {
        if (entity.x == x && entity.y == y) {
            return &entity;
        }
    }
    return nullptr;
}

bool create_player_entity(Level &level, int x, int y) {
    Entity player;
    player.x = x;
    player.y = y;
    player.type = EntityType::PLAYER;
    player.name = "Player";
    player.health = 100;
    level.entities.push_back(player);
    return true;
}