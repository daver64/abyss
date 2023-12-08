#include "abyss.h"

int32 add_entity(gamelevel *level,game_entity_type type, ivec2 position, int32 health)
{
    gameentity e;
    e.type=type;
    e.position= position;
    e.health=health;
    level->entities.emplace_back(e);
    return 0;
}