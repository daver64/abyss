#include "abyss.h"

context *app_context{nullptr};
texture *logo{nullptr};
texture *sprite1{nullptr};
array_buffer *db{nullptr};
textureatlas *atlas{nullptr};
textureatlas *tileatlas{nullptr};
soundobject *music{nullptr};
soundobject *sfx1{nullptr};


gamelevel *level{nullptr};
void update_and_render(const float64 delta_t_ms)
{
	clear_screen(x11colours::black);
	ortho2d(app_context->width, app_context->height, false, -1.0f, 1.0f);
	disable_depthtest();
	enable_blending();


	render_level(level,0,0,10,10);

	// draw an animated sprite
	bind_texture(db, logo);
	begin_quads(db);
	static float32 xpos = 10.0f;
	static float32 ypos = 300.0f;
	static float32 pxpos = 10.0f;
	ypos -= 4.0f;
	pxpos +=1.0f;
	rectangle(db, xpos, ypos, 24.0f, 24.0f, x11colours::white);
	if (ypos < 10 && pxpos<350)
	{
		ypos=300.0f;
		xpos=pxpos+12;
		//play_sound(sfx1);
	}
	if(pxpos>400)
	{
		pxpos=10.0f;
	}
	end_quads(db);

	bind_texture(db,sprite1);
	begin_quads(db);
	rectangle(db,pxpos,300,32,32,x11colours::white);
	end_quads(db);

	gprintf(atlas,0,0,x11colours::green,"frame delta t ms=%2.0lf, raw getkey=%d",
		std::floor(get_frame_delta_t_ms()),getkey(0));

}

int main(int argc, char *argv[])
{

	app_init();
	do
	{
		process_input(app_context);
		update_and_render(get_frame_delta_t_ms());

		draw_gui();
		swap(app_context);

	} while (!want_to_quit(app_context));

	app_shutdown();

	return 0;
}

void app_init()
{
	create_windowed_context(&app_context,900,600, "Abyss");
	create_texture(&logo, "../data/textures/beams.png");
	create_texture(&sprite1,"../data/textures/red1.png");
	create_array_buffer(&db);
	create_atlas(&atlas, db, "../data/textures/whitefont.png", 32, 8);
	create_atlas(&tileatlas,db,"../data/textures/scifitiles-sheet.png",14,6);
	keyboard_install(app_context);
	
	init_gui(app_context);

	create_soundobject(&sfx1,"../data/samples/cloth-inventory.wav");
	create_soundobject(&music,"../data/samples/song18.flac");
	//play_sound(music);

	create_level(&level,db,tileatlas,80,25);
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
