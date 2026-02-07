#include "../include/abyss.h"

Tile &get_tile(Level &level, int x, int y) {
    return level.tiles[y * level.width + x];
}

bool is_feature_walkable(FeatureType feature) {
    switch (feature) {
        case FeatureType::NONE:
        case FeatureType::TREE:
        case FeatureType::ROCK:
        case FeatureType::FLOWER:
        case FeatureType::SHRUB:
        case FeatureType::FOUNTAIN:
        case FeatureType::STATUE:
        case FeatureType::ALTAR:
        case FeatureType::BRIDGE:
        case FeatureType::SIGN:
        case FeatureType::LAMPPOST:
        case FeatureType::BENCH:
        case FeatureType::WELL:
        case FeatureType::POND:
            return true;
        default:
            return false;
    }
}
bool is_walkable(Tile &tile) {
    if (tile.type == TileType::FLOOR && is_feature_walkable(tile.feature)) {
        return true;
    }
    return false;
}