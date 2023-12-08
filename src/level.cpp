#include "abyss.h"

gamelevel *current_level{nullptr};

uint32 getkey(uint32 i);
void keyboard_install(context *ctx);

int32 create_level(gamelevel **level, int32 width, int32 height)
{
	assert(width>=0 && width<=4096);
	assert(height>=0 && height<=4096);
	(*level) = new gamelevel;
	(*level)->tiles.reserve(width * height);
	(*level)->width = width;
	(*level)->height = height;

	//create_atlas(&(*level)->tileatlas, drawbuffer *target, const std::string &filename, int32 numx, int32 numy);
    // add player
    add_entity( (*level), game_entity_type::entity_player);
	return 0;
}

int32 destroy_level(gamelevel **level)
{
	delete (*level);
	(*level) = nullptr;
	return 0;
}

gametile *get_tile(gamelevel *level, int32 x, int32 y)
{
	assert(x >= 0 && x < level->width);
	assert(y >= 0 && y < level->height);
	return &level->tiles[y * level->width + x];
}

int32 render_level(gamelevel *level, int32 x, int32 y, int32 width, int32 height)
{
	assert(x>=0 && width>0);
	assert(y>=0 && height>0);
	assert(x+width < level->width);
	assert(y+height<level->height);
	
	for(int32 ypos=0;ypos<height;ypos++)
	{
		for(int32 xpos=0;xpos<width;xpos++)
		{
			gametile *tile = get_tile(level,x+xpos,y+ypos);
		}
	}
	return 0;
}