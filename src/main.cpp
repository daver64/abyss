#include "abyss.h"
#include <cstdlib>
#include <cstdio>

int main(int argc, char *argv[])
{
    context *app_context{nullptr};
    int32 result=create_context(&app_context,"Abyss", 800,600,false);

    texture *logo=create_texture("../data/textures/bud.png");
    do {
        process_input(app_context);
        clear_screen(x11colours::tomato);
        swap(app_context);
        poll_input(app_context);
    } while(!want_to_quit(app_context));

    destroy_texture(logo);
    destroy_context(&app_context);
    return 0;
}