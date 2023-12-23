
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "sl.h"
ma_engine *engine{nullptr};
ma_engine_config config;

int32 create_soundobject(soundobject **sobj, const std::string &filename)
{
    if (!engine)
    {
        init_sound();
    }
    (*sobj) = (soundobject *)global_alloc(sizeof(soundobject));  // new soundobject;
    (*sobj)->sound = (ma_sound *)global_alloc(sizeof(ma_sound)); // new ma_sound;
    ma_result result = ma_sound_init_from_file(engine, filename.c_str(), 0, NULL, NULL, (*sobj)->sound);
    if (result != MA_SUCCESS)
    {
        fprintf(stderr, "could not load %s\n", filename.c_str());
        // delete (*sobj)->sound;
        // delete (*sobj);
        global_free((*sobj)->sound);
        global_free((*sobj));
        (*sobj) = nullptr;
        return -1;
    }
    return 0;
}
void destroy_soundobject(soundobject **sobj)
{
    ma_sound_stop((*sobj)->sound);
    ma_sound_uninit((*sobj)->sound);
    // delete (*sobj)->sound;
    // delete (*sobj);
    global_free((*sobj)->sound);
    global_free((*sobj));
    (*sobj) = nullptr;
}
int32 play_sound(soundobject *sobj)
{
    if (!engine)
        return -1;
    if (ma_sound_is_playing(sobj->sound))
    {
        ma_sound_stop(sobj->sound);
        ma_sound_seek_to_pcm_frame(sobj->sound, 0);
    }
    ma_sound_set_volume(sobj->sound, 0.15);
    ma_result result = ma_sound_start(sobj->sound);
    if (result != MA_SUCCESS)
    {
        return -2;
    }
    return 0;
}

int32 init_sound()
{
    engine = (ma_engine*)global_alloc(sizeof(ma_engine));//new ma_engine;
    config = ma_engine_config_init();
    ma_result result = ma_engine_init(&config, engine);
    if (result != MA_SUCCESS)
    {
        //delete engine;
        global_free(engine);
        engine = nullptr;
        fprintf(stderr, "failed to init audio engine\n");
        return -1;
    }
    return 0;
}
void deinit_sound()
{
    if (engine)
        ma_engine_uninit(engine);
}
