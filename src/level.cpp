#include "../include/abyss.h"

#include <cstdlib>
#include <ctime>
#include <vector>

namespace {
    struct Room {
        int x, y, w, h;
    };

    void set_tile(Level &level, int x, int y, TileType type) {
        if (x > 0 && x < level.width - 1 && y > 0 && y < level.height - 1) {
            Tile &tile = level.tiles[y * level.width + x];
            tile.type = type;
            tile.feature = FeatureType::NONE;
        }
    }

    void carve_room(Level &level, const Room &room) {
        for (int y = room.y; y < room.y + room.h; ++y) {
            for (int x = room.x; x < room.x + room.w; ++x) {
                set_tile(level, x, y, TileType::FLOOR);
            }
        }
    }

    void carve_horizontal_corridor(Level &level, int x1, int x2, int y) {
        int start = x1 < x2 ? x1 : x2;
        int end = x1 < x2 ? x2 : x1;
        for (int x = start; x <= end; ++x) {
            set_tile(level, x, y, TileType::FLOOR);
        }
    }

    void carve_vertical_corridor(Level &level, int x, int y1, int y2) {
        int start = y1 < y2 ? y1 : y2;
        int end = y1 < y2 ? y2 : y1;
        for (int y = start; y <= end; ++y) {
            set_tile(level, x, y, TileType::FLOOR);
        }
    }

    void connect_rooms(Level &level, const Room &room1, const Room &room2) {
        int x1 = room1.x + room1.w / 2;
        int y1 = room1.y + room1.h / 2;
        int x2 = room2.x + room2.w / 2;
        int y2 = room2.y + room2.h / 2;

        if (rand() % 2 == 0) {
            carve_horizontal_corridor(level, x1, x2, y1);
            carve_vertical_corridor(level, x2, y1, y2);
        } else {
            carve_vertical_corridor(level, x1, y1, y2);
            carve_horizontal_corridor(level, x1, x2, y2);
        }
    }
}

bool create_dungeon_level(Level &level, int width, int height)
{
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    level.width = width;
    level.height = height;
    level.type = LevelType::DUNGEON;
    level.tiles.resize(width * height);

    // Fill everything with walls
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            set_tile(level, x, y, TileType::WALL);
        }
    }

    // Ensure edges are walls (since set_tile excludes them, we set them directly)
    for (int x = 0; x < width; ++x) {
        level.tiles[0 * width + x].type = TileType::WALL;
        level.tiles[0 * width + x].feature = FeatureType::NONE;
        level.tiles[(height - 1) * width + x].type = TileType::WALL;
        level.tiles[(height - 1) * width + x].feature = FeatureType::NONE;
    }
    for (int y = 0; y < height; ++y) {
        level.tiles[y * width + 0].type = TileType::WALL;
        level.tiles[y * width + 0].feature = FeatureType::NONE;
        level.tiles[y * width + (width - 1)].type = TileType::WALL;
        level.tiles[y * width + (width - 1)].feature = FeatureType::NONE;
    }

    // Divide level into 3 columns x 2 rows of regions (with padding for edges)
    int cols = 3;
    int rows = 2;
    int region_width = (width - 2) / cols;
    int region_height = (height - 2) / rows;

    std::vector<Room> rooms;
    const int min_room_size = 4;
    const int max_room_size = 8;

    // Create rooms in each region
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // 70% chance to generate a room in this region
            if (rand() % 10 < 7) {
                int region_x = col * region_width + 2;
                int region_y = row * region_height + 2;

                int room_width = min_room_size + (rand() % (max_room_size - min_room_size + 1));
                int room_height = min_room_size + (rand() % (max_room_size - min_room_size + 1));

                // Constrain room to region with padding
                int max_x = region_x + region_width - room_width - 1;
                int max_y = region_y + region_height - room_height - 1;

                if (max_x < region_x) max_x = region_x;
                if (max_y < region_y) max_y = region_y;

                int room_x = region_x + (rand() % (max_x - region_x + 1));
                int room_y = region_y + (rand() % (max_y - region_y + 1));

                Room room = {room_x, room_y, room_width, room_height};
                carve_room(level, room);
                rooms.push_back(room);
            }
        }
    }

    // Connect rooms with corridors
    if (rooms.size() > 1) {
        for (size_t i = 0; i < rooms.size() - 1; ++i) {
            connect_rooms(level, rooms[i], rooms[i + 1]);
        }
    }

    return true;
}
bool create_cave_level(Level &level, int width, int height)
{
    level.width = width;
    level.height = height;
    level.type = LevelType::CAVE;
    level.tiles.resize(width * height);
    return true;
}
bool create_level(Level &level, int width, int height, LevelType type)
{
    switch (type)
    {
    case LevelType::DUNGEON:
        return create_dungeon_level(level, width, height);
    case LevelType::CAVE:
        return create_cave_level(level, width, height);

    }
    return false;
}