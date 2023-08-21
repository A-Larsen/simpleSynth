#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>

#include "BasicOscillator.h"
#include "playWav.h"
#include "audio.h"
#include "readWav.h"

#define M_PI ((double)3.14159265359)
#define TWOPI (M_PI + M_PI)

bool AUDIO_START = false;
float master_amp = 0.8f;

uint8_t OSCILLATOR_ID = 1;
uint8_t WAVEFILE_ID = 1;

__declspec(dllexport) int loadWav(lua_State *L)
{
    char *file_path = (char *)luaL_checkstring(L, 1);
    WavHeader *wavheader = (WavHeader *)lua_newuserdata(L, sizeof(WavHeader));
    WavData *wavdata = (WavData *)lua_newuserdata(L, sizeof(WavData));

    WAVEFORMATEX *format = (WAVEFORMATEX *)lua_newuserdata(L, 
                                 sizeof(WAVEFORMATEX));
    WavPlayer *userdata = (WavPlayer *)lua_newuserdata(L,
                                 sizeof(WavPlayer));
    char name[50];
    sprintf(name, "%s_%d", "WAVE_FILE", WAVEFILE_ID);
    lua_setglobal(L, name);

    userdata->data = NULL;

    readWav(file_path, wavheader, &userdata->data, &userdata->data_len, L);

    format->wFormatTag = wavheader->wFormatTag;
    format->nChannels = wavheader->nChannels;
    format->nSamplesPerSec = wavheader->nSamplesPerSec;
    format->wBitsPerSample = wavheader->wBitsPerSample;
    format->cbSize = wavheader->cbSize;
    userdata->format = format;
    userdata->pos = 0;

    userdata->max_amp = pow(2, format->wBitsPerSample - 1) - 1;
    wav_init(wavdata, WavPlayer_initStream, WavPlayer_handleStream, format, userdata);
    userdata->wavdata = wavdata;
    lua_pushnumber(L, (lua_Number)WAVEFILE_ID);
    WAVEFILE_ID++;
    return 1;
}
__declspec(dllexport) int playWav(lua_State *L)
{
    int id = (int)luaL_checknumber(L, 1);
    char name[50];
    sprintf(name, "%s_%d", "WAVE_FILE", id);
    lua_getglobal(L, name);
    WavPlayer *userdata = (WavPlayer *)lua_touserdata(L,  -1);
    userdata->wavdata->play = true;
    return 0;
}

__declspec(dllexport) int playOscillator(lua_State *L)
{
    int id = (int)luaL_checknumber(L, 1);
    char name[50];
    sprintf(name, "%s_%d", "BASIC_OSCILLATOR", id);
    lua_getglobal(L, name);
    BasicOscillator *userdata = (BasicOscillator *)lua_touserdata(L,  -1);
    BasicOscillator_play(userdata, true);
    return 0;
}

__declspec(dllexport) int loadOsillator(lua_State *L)
{
    char *osc = (char *)luaL_checkstring(L,  1);
    float freq = (float)luaL_checknumber(L,  2);
    float amp = (float)luaL_checknumber(L,  3);

    /* /1* WAVEFORMATEX *format = (WAVEFORMATEX *)lua_newuserdata(L, *1/ */ 
    /*                              sizeof(WAVEFORMATEX)); */
    BasicOscillator *userdata = (BasicOscillator *)lua_newuserdata(L,
                                 sizeof(BasicOscillator));
    char name[50];
    sprintf(name, "%s_%d", "BASIC_OSCILLATOR", OSCILLATOR_ID);
    lua_setglobal(L, name);

    /* WavData *wavdata = (WavData *)lua_newuserdata(L, sizeof(WavData)); */

    /* format->wFormatTag = WAVE_FORMAT_PCM; */
    /* format->nChannels = 1; */
    /* format->nSamplesPerSec = SAMPLING_RATE; */
    /* format->wBitsPerSample = 16; */
    /* format->cbSize = 0; */
    /* userdata->frequency = 400; */
    /* userdata->wave_position = 0; */
    /* userdata->wave_step = 0; */
    /* userdata->amplitude = 0.0f; */
    /* userdata->type = OSCILLATOR_SINE; */
    /* userdata->max_amp = (32767 * userdata->amplitude); */
    /* userdata->wavdata = wavdata; */
    /* userdata->frequency = freq; */
    /* userdata->amplitude = amp; */
    /* userdata->max_amp = (32767 * userdata->amplitude); */
    /* userdata->base_pitch = 48; */
    /* userdata->amp_step = 0.01f; */

    BasicOscillator_init(userdata);
    /* wav_init(wavdata, BasicOscillator_initStream, BasicOscillator_handleStream, format, userdata); */

    if (strcmp(osc, "sine") == 0) {
        BasicOscillator_setType(userdata, OSCILLATOR_SINE);
    }
    if (strcmp(osc, "saw") == 0) {
        BasicOscillator_setType(userdata, OSCILLATOR_SAW);
    }
    if (strcmp(osc, "square") == 0) {
        BasicOscillator_setType(userdata, OSCILLATOR_SQUARE);
    }

    lua_pushnumber(L, (lua_Number)OSCILLATOR_ID);
    OSCILLATOR_ID++;
    return 1;
}
__declspec(dllexport) int masterAmp(lua_State *L)
{
    master_amp = (float)luaL_checknumber(L, 1);
    return 0;
}

__declspec(dllexport) int play(lua_State *L)
{
    AUDIO_START = true;
    return 0;
}

__declspec(dllexport) luaL_Reg audio[] = {
    {"loadOsillator", loadOsillator},
    {"loadWav", loadWav},
    {"playWav", playWav},
    {"playOscillator", playOscillator},
    {"play", play},
    {"masterAmp", masterAmp},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_audio_lualib(lua_State *L)
{
    luaL_newlib(L, audio);
    return 1;
}
