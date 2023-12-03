#include "abyss.h"
#include <cstdlib>
#include <cstdio>

int main(int argc, char* argv[])
{
	context* app_context{ nullptr };
	int32 result = create_context(&app_context, "Abyss", 800, 600, false);

	enable_texturing();
	enable_multisampling();
	enable_texture_filtering();

	texture* logo = create_texture("../data/textures/bud.png");
	drawbuffer* db = create_drawbuffer();

	do {
		process_input(app_context);
		clear_screen(x11colours::tomato);
		ortho2d(800, 600, false, -1.0f, 1.0f);
		disable_depthtest();
		bind_texture(db, logo);
		begin_quads(db);
		draw_rectangle(db, 100.0f, 100.0f, 64.0f, 64.0f, x11colours::white);
		end_quads(db);

		swap(app_context);
		poll_input(app_context);
	} while (!want_to_quit(app_context));

	destroy_texture(logo);
	destroy_drawbuffer(db);
	destroy_context(&app_context);
	return 0;
}