#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>

#include "audio.h"
#include "readWav.h"

#define M_PI ((double)3.14159265359)
#define TWOPI (M_PI + M_PI)

bool AUDIO_START = false;

typedef struct _BasicOscillators {
    float frequency;
    float wave_position;
    float wave_step;
    short type;
    float amplitude;
    float max_amp;
} BasicOsilators;

typedef struct _WavPlayer {
    float max_amp;
    int16_t *data;
    uint32_t data_len;
    uint32_t pos;
    WAVEFORMATEX *format;
} WavPlayer;

typedef enum _Oscillator_type{
    OSCILLATOR_SINE = 0,
    OSCILLATOR_SAW,
    OSCILLATOR_SQUARE,
}Oscillator_type;


__declspec(dllexport) void setStep(BasicOsilators *userdata)
{
    switch(userdata->type) {
        case OSCILLATOR_SINE: 
        {
            userdata->wave_step = TWOPI / ((float)SAMPLING_RATE / userdata->frequency);
            break;
        }
        case OSCILLATOR_SAW: 
        case OSCILLATOR_SQUARE: 
        {
            userdata->wave_step = 1 / ((float)SAMPLING_RATE / userdata->frequency);
            break;
        }
    }

}

__declspec(dllexport) void initOscillatorStream(WavData *wavdata)
{
    BasicOsilators *userdata = (BasicOsilators *)wavdata->data;
    setStep(userdata);
}
__declspec(dllexport) void initWavPlayerStream(WavData *wavdata)
{

}
__declspec(dllexport) void handleWavPlayerStream(int16_t *stream, WavData *wavdata)
{
    WavPlayer *userdata = (WavPlayer *)wavdata->data;
    if(userdata->pos > userdata->data_len) {
        return;
    }
    *stream = userdata->data[userdata->pos];
    userdata->pos += 1;

}

__declspec(dllexport) void handleOscillatorStream(int16_t *stream, WavData *wavdata)
{
    BasicOsilators *userdata = (BasicOsilators *)wavdata->data;
    switch(userdata->type) {
        case OSCILLATOR_SINE:
        {
            *stream = sin(userdata->wave_position) * userdata->max_amp;
            if (userdata->wave_position >= TWOPI) userdata->wave_position = 0;
            break;
        }
        case OSCILLATOR_SAW:
        {
            *stream = (userdata->wave_position) *userdata->max_amp;
            if (userdata->wave_position >= 1) userdata->wave_position = 0;
            break;
        }
        case OSCILLATOR_SQUARE:
        {
            *stream = round(userdata->wave_position) *  userdata->max_amp;
            if (userdata->wave_position >= 1) userdata->wave_position = 0;
            break;
        }
    }
    userdata->wave_position += userdata->wave_step;
}

__declspec(dllexport) int loadWav(lua_State *L)
{
    /* WavHeader wavheader; */
    char *file_path = (char *)luaL_checkstring(L, 1);
    WavHeader *wavheader = (WavHeader *)lua_newuserdata(L, sizeof(WavHeader));
    WavData *wavdata = (WavData *)lua_newuserdata(L, sizeof(WavData));
    /* WavData wavdata; */

    WAVEFORMATEX *format = (WAVEFORMATEX *)lua_newuserdata(L, 
                                 sizeof(WAVEFORMATEX));
    WavPlayer *userdata = (WavPlayer *)lua_newuserdata(L,
                                 sizeof(WavPlayer));
    lua_setglobal(L, file_path);

    userdata->data = NULL;

    readWav("seaShells.wav", wavheader, &userdata->data, &userdata->data_len, L);

    format->wFormatTag = wavheader->wFormatTag;
    format->nChannels = wavheader->nChannels;
    format->nSamplesPerSec = wavheader->nSamplesPerSec;
    format->wBitsPerSample = wavheader->wBitsPerSample;
    format->cbSize = wavheader->cbSize;
    userdata->format = format;
    userdata->pos = 0;

    userdata->max_amp = pow(2, format->wBitsPerSample - 1) - 1;
    wav_init(wavdata, initWavPlayerStream, handleWavPlayerStream, format, userdata);

    return 0;
}

__declspec(dllexport) int frewWav(lua_State *L)
{
    return 0;
}
__declspec(dllexport) int playOScillator(lua_State *L)
{
    lua_getglobal(L, "BASIC_OSCILLATORS");
    BasicOsilators *userdata = (BasicOsilators *)lua_touserdata(L,  -1);
    lua_pop(L, 1);
    char *osc = (char *)luaL_checkstring(L,  1);
    float freq = (float)luaL_checknumber(L,  2);
    float amp = (float)luaL_checknumber(L,  3);
    userdata->frequency = freq;

    if (strcmp(osc, "sine") == 0) {
        userdata->type = OSCILLATOR_SINE;
        setStep(userdata);
    }
    if (strcmp(osc, "saw") == 0) {
        userdata->type = OSCILLATOR_SAW;
        setStep(userdata);
    }
    if (strcmp(osc, "square") == 0) {
        userdata->type = OSCILLATOR_SQUARE;
        setStep(userdata);
    }

    userdata->amplitude = amp;
    userdata->max_amp = (32767 * userdata->amplitude);
    return 0;
}
__declspec(dllexport) int play(lua_State *L)
{
    AUDIO_START = true;
    return 0;
}

__declspec(dllexport) luaL_Reg audio[] = {
    {"playOscillator", playOScillator},
    {"loadWav", loadWav},
    {"play", play},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_audio_lualib(lua_State *L)
{
    WAVEFORMATEX *format = (WAVEFORMATEX *)lua_newuserdata(L, 
                                 sizeof(WAVEFORMATEX));
    BasicOsilators *userdata = (BasicOsilators *)lua_newuserdata(L,
                                 sizeof(BasicOsilators));
    format->wFormatTag = WAVE_FORMAT_PCM;
    format->nChannels = 1;
    format->nSamplesPerSec = SAMPLING_RATE;
    format->wBitsPerSample = 16;
    format->cbSize = 0;
    userdata->frequency = 400;
    userdata->wave_position = 0;
    userdata->wave_step = 0;
    userdata->amplitude = 0.0f;
    userdata->type = OSCILLATOR_SINE;
    userdata->max_amp = (32767 * userdata->amplitude);
    lua_setglobal(L, "BASIC_OSCILLATORS");
    WavData *wavdata = (WavData *)lua_newuserdata(L, sizeof(WavData));
    wav_init(wavdata, initOscillatorStream, handleOscillatorStream, format, userdata);
    luaL_newlib(L, audio);
    return 1;
}
