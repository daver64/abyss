#include "abyss.h"
#include <cstdlib>
#include <cstdio>

int main(int argc, char* argv[])
{
	context* app_context{ nullptr };
	int32 result = create_context(&app_context, "Abyss", 800, 600, false);

	enable_texturing();

	enable_texture_filtering();
	texture* logo = create_texture("../data/textures/bud.png");
	drawbuffer* db = new drawbuffer;

	do {
		process_input(app_context);
		clear_screen(x11colours::tomato);
		ortho2d(800, 600, false, 0.5, 200.0);
		disable_depthtest();
		texture_bind(logo);
		db->bind_tex0(logo->ref);
		db->begin_quads();
		db->rect(100.0f, 100.0f, 128.0f, 128.0f, x11colours::white);
		db->end();
		swap(app_context);
		poll_input(app_context);
	} while (!want_to_quit(app_context));

	destroy_texture(logo);
	delete db;
	destroy_context(&app_context);
	return 0;
}