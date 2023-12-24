#pragma once
#pragma once

#include "sl.h"

enum class game_input {
	move_up,move_down,move_left,move_right
};

enum class game_entity_type {
	entity_none,entity_player, entity_alien, entity_plant, entity_door, entity_switch
};

enum class game_tile_type {
	tile_stone_floor, tile_stone_wall, tile_dirt_floor, tile_dirt_wall
};

struct gameentity {
	game_entity_type type{game_entity_type::entity_none};
	ivec2 position{0,0};
	int32 health{0};
};

struct gametile {
	game_tile_type type{game_tile_type::tile_stone_floor};
	int32 movement_cost{0};
};

struct gamelevel {
	std::vector<gametile> tiles;
	std::vector<gameentity> entities;
	int32 width{0};
	int32 height{0};
	textureatlas *tileatlas{nullptr};
	array_buffer *target{nullptr};
};

extern gamelevel *current_level;

void app_init();
void app_shutdown();
void update_and_render(const float64 delta_t_ms);

int32 create_level(gamelevel **level, array_buffer *targetbuffer, textureatlas *atlas,int32 width, int32 height);
int32 destroy_level(gamelevel **level);
gametile *get_tile(gamelevel *level, int32 x, int32 y);
int32 add_entity(gamelevel *level,game_entity_type type, ivec2 position = ivec2{10,10}, int32 health=10);
int32 render_level(gamelevel *level, int32 x, int32 y, int32 width, int32 height, int32 offset_x=32,int32 offset_y=32);