#ifndef _BASIC_OSCILLATOR_H_
#define _BASIC_OSCILLATOR_H_

#include "audio.h"
#include "audio-tools.h"
#include <stdint.h>

#define M_PI ((double)3.14159265359)
#define TWOPI (M_PI + M_PI)

#define KEYNOTES "awsedftgyhujkolp;\'"
#define KEYNOTES_LEN 18

typedef struct _BasicOscillator {
    float frequency;
    float wave_position;
    float wave_step;
    short type;
    float amplitude;
    float max_amp;
    float amp_step;
    float base_pitch;
} BasicOscillator;

typedef enum _Oscillator_type{
    OSCILLATOR_SINE = 0,
    OSCILLATOR_SAW,
    OSCILLATOR_SQUARE,
}Oscillator_type;

static void _BasicOscillator_setStep(BasicOscillator *userdata)
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
void BasicOscillator_setType(BasicOscillator *userdata, Oscillator_type type)
{
    userdata->type = type;
    _BasicOscillator_setStep(userdata);
}

void BasicOscillator_ampDown(BasicOscillator *userdata)
{
    if (userdata->amplitude - userdata->amp_step <= 0 ) {
        return;
    }
    userdata->amplitude -= userdata->amp_step;
    userdata->max_amp = (32767 * userdata->amplitude);

}
void BasicOscillator_ampUp(BasicOscillator *userdata)
{
    if (userdata->amplitude + userdata->amp_step >= 1 ) {
        return;
    }
    userdata->amplitude += userdata->amp_step;
    userdata->max_amp = (32767 * userdata->amplitude);

}

void BasicOscillator_initStream(WavData *wavdata)
{
    BasicOscillator *userdata = (BasicOscillator *)wavdata->data;
    _BasicOscillator_setStep(userdata);
}

void BasicOscillator_setPitch(BasicOscillator *userdata, uint16_t p)
{
    userdata->frequency = pitchToFrequency(p);
    _BasicOscillator_setStep(userdata);
}
void BasicOscillator_mapKey(BasicOscillator *userdata, char key)
{
    for (int i = 0; i < KEYNOTES_LEN; ++i) {
        if(KEYNOTES[i] == key) {
            BasicOscillator_setPitch(userdata, userdata->base_pitch + i);
        }
    }
}
void BasicOscillator_setBasePitch(BasicOscillator *userdata, uint16_t base)
{
    userdata->base_pitch = base;
}

void BasicOscillator_handleStream(int16_t *stream, WavData *wavdata)
{
    BasicOscillator *userdata = (BasicOscillator *)wavdata->data;
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



#endif // _BASIC_OSCILLATOR_H_
