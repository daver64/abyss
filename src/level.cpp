#include "../include/abyss.h"

bool create_level(Level &level, int width, int height, LevelType type)
{
    level.width = width;
    level.height = height;
    level.type = type;
    level.tiles.resize(width * height);
    return true;
}