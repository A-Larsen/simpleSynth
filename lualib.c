#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>
#include "audio.h"

#define M_PI ((double)3.14159265359)
#define TWOPI (M_PI + M_PI)


typedef struct _UserData {
    float frequency;
    float wave_position;
    float wave_step;
    short type;
    float amplitude;
    float max_amp;
} UserData;

typedef enum _Oscillator_type{
    OSCILLATOR_SINE = 0,
    OSCILLATOR_SAW,
    OSCILLATOR_SQUARE,
}Oscillator_type;


__declspec(dllexport) void setStep(UserData *userdata)
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

__declspec(dllexport) void initStream(WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
    setStep(userdata);
}

__declspec(dllexport) void handleStream(int16_t *stream, WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
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

__declspec(dllexport) int playOScillator(lua_State *L)
{
    lua_getglobal(L, "USERDATA");
    UserData *userdata = (UserData *)lua_touserdata(L,  -1);
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

__declspec(dllexport) luaL_Reg audio[] = {
    {"playOscillator", playOScillator},
    {NULL, NULL}
};

__declspec(dllexport) int luaopen_audio_lualib(lua_State *L)
{
    UserData *userdata = (UserData *)lua_newuserdata(L, sizeof(UserData));
    userdata->frequency = 400;
    userdata->wave_position = 0;
    userdata->wave_step = 0;
    userdata->amplitude = 0.0f;
    userdata->type = OSCILLATOR_SINE;
    userdata->max_amp = (32767 * userdata->amplitude);
    lua_setglobal(L, "USERDATA");
    WavData *wavdata = (WavData *)lua_newuserdata(L, sizeof(WavData));
    lua_setglobal(L, "WAVDATA");
    wav_init(wavdata, initStream, handleStream, userdata);
    luaL_newlib(L, audio);
    return 1;
}
