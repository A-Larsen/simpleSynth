#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>

#include "BasicOscillator.h"
#include "playWav.h"
#include "audio.h"
#include "readWav.h"
#include "mixer.h"

#define M_PI ((double)3.14159265359)
#define TWOPI (M_PI + M_PI)

bool AUDIO_START = false;
Mixer mixer;

__declspec(dllexport) int loadWav(lua_State *L)
{
    static uint8_t WAVEFILE_COUNT = 1;
    const char *file_path = (const char *)luaL_checkstring(L, 1);
    WavPlayer *userdata = (WavPlayer *)lua_newuserdata(L,
                                 sizeof(WavPlayer));
    char name[50];
    sprintf(name, "%s_%d", "WAVE_FILE", WAVEFILE_COUNT);
    lua_setglobal(L, name);

    WavPlayer_init(userdata, file_path, L);
    lua_pushnumber(L, (lua_Number)WAVEFILE_COUNT);
    WAVEFILE_COUNT++;
    return 1;
}
__declspec(dllexport) int playWav(lua_State *L)
{
    int id = (int)luaL_checknumber(L, 1);
    char name[50];
    sprintf(name, "%s_%d", "WAVE_FILE", id);
    lua_getglobal(L, name);
    WavPlayer *userdata = (WavPlayer *)lua_touserdata(L,  -1);
    WavPlayer_play(userdata, true);
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
    static uint8_t OSCILLATOR_COUNT = 1;
    char *osc = (char *)luaL_checkstring(L,  1);
    float freq = (float)luaL_checknumber(L,  2);
    float amp = (float)luaL_checknumber(L,  3);

    BasicOscillator *userdata = (BasicOscillator *)lua_newuserdata(L,
                                 sizeof(BasicOscillator));
    char name[50];
    sprintf(name, "%s_%d", "BASIC_OSCILLATOR", OSCILLATOR_COUNT);
    lua_setglobal(L, name);


    BasicOscillator_init(userdata);
    BasicOscillator_setFreq(userdata, freq);
    BasicOscillator_setAmp(userdata, amp);

    if (strcmp(osc, "sine") == 0) {
        BasicOscillator_setType(userdata, OSCILLATOR_SINE);
    }
    if (strcmp(osc, "saw") == 0) {
        BasicOscillator_setType(userdata, OSCILLATOR_SAW);
    }
    if (strcmp(osc, "square") == 0) {
        BasicOscillator_setType(userdata, OSCILLATOR_SQUARE);
    }

    lua_pushnumber(L, (lua_Number)OSCILLATOR_COUNT);
    OSCILLATOR_COUNT++;
    return 1;
}

__declspec(dllexport) int masterAmp(lua_State *L)
{
    mixer.master_amp = (float)luaL_checknumber(L, 1);
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
    mixer.master_amp = 0.8f;
    luaL_newlib(L, audio);
    return 1;
}
