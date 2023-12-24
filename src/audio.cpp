
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "sl.h"
ma_engine *engine{nullptr};
ma_engine_config config;

#define ERROR_INIT_SOUND_FAILURE (-1)
#define ERROR_CREATE_SOUNDOBJECT_FILE_ERROR (-1)
#define ERROR_CREATE_SOUNDOBJECT_OBJECT_ALLOC (-3)
#define ERROR_CREATE_SOUNDOBJECT_ALLOC (-2)
#define ERROR_
#define ERROR_PLAY_SOUND (-1)

#define CREATE_SOUNDOBJECT_OK (0)
#define PLAY_SOUND_OK (0)
#define INIT_SOUND_OK (0)

int32 create_soundobject(soundobject **sobj, const std::string &filename)
{
    assert(engine);
    (*sobj) = (soundobject *)global_alloc(sizeof(soundobject),"sound object");
    if(!(*sobj))
    {
        return ERROR_CREATE_SOUNDOBJECT_ALLOC;
    }
    (*sobj)->sound = (ma_sound *)global_alloc(sizeof(ma_sound),"sound object sound");
    if(!(*sobj)->sound)
    {
        global_free((*sobj));
        (*sobj)=nullptr;
        return ERROR_CREATE_SOUNDOBJECT_OBJECT_ALLOC;
    }
    ma_result result = ma_sound_init_from_file(engine, filename.c_str(), 0, NULL, NULL, (*sobj)->sound);
    if (result != MA_SUCCESS)
    {
        fprintf(stderr, "could not load %s\n", filename.c_str());
        global_free((*sobj)->sound);
        global_free((*sobj));
        (*sobj) = nullptr;
        return ERROR_CREATE_SOUNDOBJECT_FILE_ERROR;
    }
    return CREATE_SOUNDOBJECT_OK;
}
void destroy_soundobject(soundobject **sobj)
{
    ma_sound_stop((*sobj)->sound);
    ma_sound_uninit((*sobj)->sound);
    global_free((*sobj)->sound);
    global_free((*sobj));
    (*sobj) = nullptr;
}
int32 play_sound(soundobject *sobj)
{
    if (ma_sound_is_playing(sobj->sound))
    {
        ma_sound_stop(sobj->sound);
        ma_sound_seek_to_pcm_frame(sobj->sound, 0);
    }
    ma_sound_set_volume(sobj->sound, 0.5);
    ma_result result = ma_sound_start(sobj->sound);
    if (result != MA_SUCCESS)
    {
        fprintf(stderr,"could not start sound object\n");
        return ERROR_PLAY_SOUND;
    }
    return PLAY_SOUND_OK;
}

int32 init_sound()
{
    engine = (ma_engine*)global_alloc(sizeof(ma_engine),"sound engine object");//new ma_engine;
    config = ma_engine_config_init();
    ma_result result = ma_engine_init(&config, engine);
    if (result != MA_SUCCESS)
    {
        //delete engine;
        global_free(engine);
        engine = nullptr;
        fprintf(stderr, "failed to init audio engine\n");
        return ERROR_INIT_SOUND_FAILURE;
    }
    return INIT_SOUND_OK;
}
void deinit_sound()
{
    assert(engine);
    ma_engine_uninit(engine);
}
