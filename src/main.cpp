#include "abyss.h"

context *app_context{nullptr};
texture *logo{nullptr};
texture *sprite1{nullptr};
drawbuffer *db{nullptr};
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
	draw_rectangle(db, xpos, ypos, 24.0f, 24.0f, x11colours::white);
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
	draw_rectangle(db,pxpos,300,32,32,x11colours::white);
	end_quads(db);

	gprintf(atlas,0,0,x11colours::green,"frame delta t ms=%2.2lf, raw getkey=%d",get_frame_delta_t_ms(),getkey(0));
	// bitmapped font plain text output.
	//gprintf(atlas, 64, 96, x11colours::tomato, "hello %d bit world.\tafter tab.\nanother line\n%2.2lf",
	//		getkey(0), get_frame_delta_t_ms());
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
	create_texture(&logo, "../data/textures/beams.png");
	create_texture(&sprite1,"../data/textures/red1.png");
	create_drawbuffer(&db);
	create_atlas(&atlas, db, "../data/textures/whitefont.png", 32, 8);
	create_atlas(&tileatlas,db,"../data/textures/scifitiles-sheet.png",14,6);
	keyboard_install(app_context);
	
	create_soundobject(&sfx1,"../data/samples/cloth-inventory.wav");
	create_soundobject(&music,"../data/samples/song18.mp3");
	//play_sound(music);

	create_level(&level,db,tileatlas,80,25);
}

void app_shutdown()
{
	destroy_atlas(&atlas);
	destroy_atlas(&tileatlas);
	destroy_drawbuffer(&db);
	destroy_texture(&logo);
	destroy_texture(&sprite1);
	destroy_soundobject(&music);
	destroy_soundobject(&sfx1);
	destroy_context(&app_context);
}
