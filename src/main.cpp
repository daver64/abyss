#include "abyss.h"
#include <cstdlib>
#include <cstdio>



int main(int argc, char *argv[])
{
	context *app_context{nullptr};
	texture *logo{nullptr};
	drawbuffer *db{nullptr};
	textureatlas *atlas{nullptr};

	create_context(&app_context, "Abyss", 800, 600, false);
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
		draw_rectangle(db, 10.0f, 10.0f, 64.0f, 64.0f, x11colours::white);
		end_quads(db);

		textout(atlas,"Hello world!",100,52);

		swap(app_context);

	} while (!want_to_quit(app_context));

	destroy_atlas(&atlas);
	destroy_drawbuffer(&db);	
	destroy_texture(&logo);
	destroy_context(&app_context);

	return 0;
}