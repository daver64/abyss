#pragma once

#include <string>
#include <vector>

//
// Abyss Game Engine - Core Data Structures

bool init_platform(int width, int height, const std::string &title);
bool shutdown_platform();
bool update_platform();
void quit_platform();
bool want_quit_platform();
void clear_screen();
void swap_buffers();
bool keydown_callback(int scancode);
//
// Game Data Structures
enum class ItemType{
    WEAPON,
    ARMOUR,
    POTION
};

enum class EntityType{
    PLAYER,
    MONSTER,
    NPC
};

enum class FeatureType{
    NONE,
    TREE,
    ROCK,
    CHEST,
    FLOWER,
    SHRUB,
    FOUNTAIN,
    STATUE,
    ALTAR,
    BRIDGE,
    SIGN,
    LAMPPOST,
    BENCH,
    WELL,
    POND,
    CAVE,
    RUINS,
    TOWER,
};

enum class TileType{
    FLOOR,
    WALL,
    WATER,
    LAVA,
    DOOROPEN,
    DOORCLOSED,
    EXIT,
};

enum class MaterialType{
    WOOD,
    STONE,
    METAL,
    GLASS
};

enum class LevelType{
    DUNGEON,
    CAVE,

};

struct Tile {
    TileType type;
    FeatureType feature;

};

struct Item{
    ItemType type;
    std::string name;
    int power;
};

struct Entity{
    int x,y;
    EntityType type;
    std::string name;
    int health;
    std::vector<Item> inventory;
};

struct Level{
    int width;
    int height;
    LevelType type;
    std::vector<Tile> tiles;
    std::vector<Entity> entities;
};
#include <string>

struct TextureAtlas {
    unsigned int gl_texture_id = 0;
    int width = 0;
    int height = 0;
    std::string filepath;
};
extern TextureAtlas tile_atlas;
bool load_texture_atlas(const std::string &filename, TextureAtlas &atlas);
bool unload_texture_atlas(TextureAtlas &atlas);
// tile api
Tile &get_tile(Level &level, int x, int y);
bool is_walkable(Tile &tile);
void draw_atlas_tile(const TextureAtlas &atlas, int tile_index, int tile_width, int tile_height, int screen_x, int screen_y, float z = 0.0f, float scale = 1.0f);

// entity api
bool create_player_entity(Level &level);
Entity *get_entity_at(Level &level, int x, int y);
bool move_entity(Entity &entity, Level &level, int new_x, int new_y);
bool can_move_to(Entity &entity, Level &level, int new_x, int new_y);
// level api
extern Level *g_current_level;
bool create_level(Level &level, int width, int height, LevelType type);
bool draw_level(const Level &level, float scale = 1.0f);
Entity *get_player_entity();

// Font rendering
struct RGBA {
    unsigned char r, g, b, a;
};

bool load_font(const std::string &font_path, int font_size);
bool unload_font();
int gprintf(int x, int y, RGBA colour, const char *fmt, ...);
