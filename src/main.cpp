#include "abyss.h"


int main(int argc, char *argv[])
{
	bool appquit = false;
	context *app_context{nullptr};
	texture *logo{nullptr};
	drawbuffer *db{nullptr};
	textureatlas *atlas{nullptr};

	create_context(&app_context, "Abyss");
	create_texture(&logo, "../data/textures/bud.png");
	create_drawbuffer(&db);
	create_atlas(&atlas, db, "../data/textures/whitefont.png", 32, 8);
	do
	{
		process_input(app_context);

		clear_screen(x11colours::black);
		ortho2d(800, 600, false, -1.0f, 1.0f);
		disable_depthtest();

		bind_texture(db, logo);
		begin_quads(db);
		static float32 xpos = 10.0f;
		xpos += 0.1;
		draw_rectangle(db, xpos, 10.0f, 64.0f, 64.0f, x11colours::white);
		if (xpos >= 400)
			xpos = 0.0f;
		end_quads(db);

		textout(atlas, "Hello world!", 100, 52);

		swap(app_context);

	} while (!want_to_quit(app_context));

	destroy_atlas(&atlas);
	destroy_drawbuffer(&db);
	destroy_texture(&logo);
	destroy_context(&app_context);

	return 0;
}