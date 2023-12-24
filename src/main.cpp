#include "abyss.h"

context *app_context{nullptr};
texture *logo{nullptr};
texture *sprite1{nullptr};
array_buffer *db{nullptr};
textureatlas *atlas{nullptr};
textureatlas *tileatlas{nullptr};
soundobject *music{nullptr};
soundobject *sfx1{nullptr};
bool playsounds{true};

int main(int argc, char *argv[])
{

	app_init();
	do
	{
		process_input(app_context);
		clear_screen(x11colours::black);
		ortho2d(app_context->width, app_context->height, false, -1.0f, 1.0f);
		disable_depthtest();
		enable_blending();

		draw_gui();

		gprintf(atlas, 0, 0, x11colours::green, "frame delta t ms=%2.0lf, raw getkey=%d",
				std::floor(get_frame_delta_t_ms()), getkey(0));
		swap(app_context);

	} while (!want_to_quit(app_context));

	app_shutdown();

	return 0;
}

void app_init()
{
	create_windowed_context(&app_context, 900, 600, "Abyss");
	clear_screen(x11colours::black);
	swap(app_context);
	create_texture(&logo, "../data/textures/beams.png");
	create_texture(&sprite1, "../data/textures/red1.png");
	create_array_buffer(&db);
	create_atlas(&atlas, db, "../data/textures/whitefont.png", 32, 8);
	create_atlas(&tileatlas, db, "../data/textures/scifitiles-sheet.png", 14, 6);
	keyboard_install(app_context);

	init_gui(app_context);

	create_soundobject(&sfx1, "../data/samples/cloth-inventory.wav");
	create_soundobject(&music, "../data/samples/song18.flac");
	play_sound(music);
}

void app_shutdown()
{
	destroy_atlas(&atlas);
	destroy_atlas(&tileatlas);
	destroy_array_buffer(&db);
	destroy_texture(&logo);
	destroy_texture(&sprite1);
	destroy_soundobject(&music);
	destroy_soundobject(&sfx1);
	deinit_gui();
	destroy_context(&app_context);
}
