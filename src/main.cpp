#include "abyss.h"

context *app_context{nullptr};
texture *logo{nullptr};
drawbuffer *db{nullptr};
textureatlas *atlas{nullptr};


void update_and_render(const float64 delta_t_ms)
{
	clear_screen(x11colours::black);
	ortho2d(800, 600, false, -1.0f, 1.0f);
	disable_depthtest();

	// draw an animated sprite
	bind_texture(db, logo);
	begin_quads(db);
	static float32 xpos = 10.0f;
	xpos += 1.0f;
	draw_rectangle(db, xpos, 10.0f, 64.0f, 64.0f, x11colours::white);
	if (xpos >= 400)
		xpos = 0.0f;
	end_quads(db);

	// bitmapped font plain text output.
	gprintf(atlas, 64, 96, x11colours::tomato, "hello %d bit world.\tafter tab.\nanother line\n%2.2lf",
			getkey(0), get_frame_delta_t_ms());
}

int main(int argc, char *argv[])
{

	app_init();
	do
	{
		process_input(app_context);
		update_and_render(get_frame_delta_t_ms());
		swap(app_context);

	} while (!want_to_quit(app_context));

	app_shutdown();

	return 0;
}

void app_init()
{
	create_context(&app_context, "Abyss");
	create_texture(&logo, "../data/textures/bud.png");
	create_drawbuffer(&db);
	create_atlas(&atlas, db, "../data/textures/whitefont.png", 32, 8);
	keyboard_install(app_context);
	init_sound();
	play_sound("../data/samples/song18.mp3");
}

void app_shutdown()
{
	destroy_atlas(&atlas);
	destroy_drawbuffer(&db);
	destroy_texture(&logo);
	destroy_context(&app_context);
}
