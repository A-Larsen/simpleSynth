#ifndef _BASIC_OSCILLATOR_H_
#define _BASIC_OSCILLATOR_H_
#include <stdint.h>

#include "audio.h"
#include "audio-tools.h"
#include "mixer.h"

#define M_PI ((double)3.14159265359)
#define TWOPI (M_PI + M_PI)

#define KEYNOTES "awsedftgyhujkolp;\'"
#define KEYNOTES_LEN 18


extern Mixer mixer;

typedef struct _BasicOscillator {
    float frequency;
    float wave_position;
    float wave_step;
    short type;
    float amplitude;
    float max_amp;
    float amp_step;
    float base_pitch;
    WavData wavdata;
    WAVEFORMATEX format;
} BasicOscillator;

typedef enum _Oscillator_type{
    OSCILLATOR_SINE = 0,
    OSCILLATOR_SAW,
    OSCILLATOR_SQUARE,
}Oscillator_type;

__declspec(dllexport) void BasicOscillator_setType(BasicOscillator *userdata, Oscillator_type type);
__declspec(dllexport) void BasicOscillator_ampDown(BasicOscillator *userdata);
__declspec(dllexport) void BasicOscillator_setAmp(BasicOscillator *userdata, float amp);
__declspec(dllexport) void BasicOscillator_setPitch(BasicOscillator *userdata, uint16_t p);
__declspec(dllexport) void BasicOscillator_mapKey(BasicOscillator *userdata, char key);
__declspec(dllexport) void BasicOscillator_setBasePitch(BasicOscillator *userdata, uint16_t base);
__declspec(dllexport) void BasicOscillator_setFreq(BasicOscillator *userdata, float frequency);
__declspec(dllexport) void BasicOscillator_init(BasicOscillator *userdata);
__declspec(dllexport) void BasicOscillator_play(BasicOscillator *userdata, bool play);
__declspec(dllexport) void BasicOscillator_ampUp(BasicOscillator *userdata);

#endif // _BASIC_OSCILLATOR_H_
