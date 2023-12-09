
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "abyss.h"
ma_result result;
ma_engine *engine;
bool audio_inited = false;
ma_engine_config config;
void init_sound()
{
    
    
    engine = new ma_engine;
    config = ma_engine_config_init();
    result = ma_engine_init(&config, engine);
    if (result != MA_SUCCESS)
    {
        fprintf(stderr, "failed to init audio engine\n");
        audio_inited = false;
        return;
    }
    fprintf(stderr,"audio engine init ok\n");
    audio_inited = true;
}
void deinit_sound()
{
}
void play_sound(const std::string &filename)
{
    ma_engine_play_sound(engine,filename.c_str(),NULL);
}
void play_sound1(const std::string &filename)
{
    if (!audio_inited)
        return;
    ma_result result;
    ma_sound sound;
    fprintf(stderr,"init sound from file %s\n",filename.c_str());
    result = ma_sound_init_from_file(engine, filename.c_str(), 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS)
    {
        fprintf(stderr,"could not load %s\n", filename.c_str());
    }
    else
    {
        fprintf(stderr,"playback sound\n");
        ma_sound_start(&sound);
    }
}