#include "../include/abyss.h"


int main(int argc[[maybe_unused]], char *argv[]){
    Level current_level;
    create_level(current_level, 20, 20, LevelType::DUNGEON);
    create_player_entity(current_level, 1, 1);
    return 0;
}
