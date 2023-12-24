#include "abyss.h"

int32 add_entity(gamelevel *level,game_entity_type type, ivec2 position, int32 health)
{
    gameentity e;
    e.type=type;
    e.position= position;
    e.health=health;
    level->entities[level->entity_count]=e;
    level->entity_count++;
    assert( level->entity_count<level->entity_capacity );
    return 0;
}