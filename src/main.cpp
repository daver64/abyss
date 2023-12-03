#include "abyss.h"
#include <cstdlib>
#include <cstdio>

int main(int argc, char *argv[])
{
	context *app_context{nullptr};
	int32 result = create_context(&app_context, "Abyss", 800, 600, false);
	enable_texturing();
	texture *logo{nullptr};
	result = create_texture(&logo, "../data/textures/bud.png");
	drawbuffer *db{nullptr};
	result = create_drawbuffer(&db);
	textureatlas *atlas{nullptr};
	result = create_atlas(&atlas, db, "../data/textures/whitefont.png", 32, 8);

	do
	{
		process_input(app_context);
		clear_screen(x11colours::tomato);
		ortho2d(800, 600, false, -1.0f, 1.0f);
		disable_depthtest();

		enable_texturing();
		enable_multisampling();
		enable_texture_filtering();

		bind_texture(db, logo);
		begin_quads(db);
		draw_rectangle(db, 100.0f, 100.0f, 64.0f, 64.0f, x11colours::white);
		end_quads(db);

		disable_texture_filtering();
		disable_multisampling();
		bind_atlas(atlas);
		begin_atlas(atlas);
		int32 index=(int32)'A';
		draw_atlas_tile(atlas, 200, 100, 32, 32, index, x11colours::white);
		end_atlas(atlas);

		swap(app_context);
		poll_input(app_context);
	} while (!want_to_quit(app_context));

	destroy_atlas(&atlas);
	destroy_texture(&logo);
	destroy_drawbuffer(&db);
	destroy_context(&app_context);

	return 0;
}