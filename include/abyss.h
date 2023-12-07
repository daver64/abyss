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
};

void app_init();
void app_shutdown();
void update_and_render(const float64 delta_t_ms);
