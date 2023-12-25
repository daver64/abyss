#include "sl.h"

context *app_context{nullptr};
array_buffer *db{nullptr};
textureatlas *atlas{nullptr};

int main(int argc, char *argv[])
{
	create_windowed_context(&app_context, 900, 600, "Abyss");
	create_array_buffer(&db);
	create_atlas(&atlas, db, "../data/textures/whitefont.png", 32, 8);
	keyboard_install(app_context);
	init_gui(app_context);
	do
	{
		process_input(app_context);
		clear_screen(x11colours::black);
		ortho2d(app_context);
		disable_depthtest();
		enable_blending();
		
		draw_gui();

		gprintf(atlas, 0, 0, x11colours::green, "frame delta t ms=%2.0lf, raw getkey=%d",
				std::floor(get_frame_delta_t_ms()), getkey(0));
		swap(app_context);

	} while (!want_to_quit(app_context));
	destroy_atlas(&atlas);
	destroy_array_buffer(&db);
	deinit_gui();
	destroy_context(&app_context);
	return 0;
}
