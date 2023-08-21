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
    Mixer *mixer;
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
        // multiple these by master amp in the future
        case OSCILLATOR_SINE:
        {
            *stream = sin(userdata->wave_position) * userdata->max_amp * userdata->mixer->master_amp;
            if (userdata->wave_position >= TWOPI) userdata->wave_position = 0;
            break;
        }
        case OSCILLATOR_SAW:
        {
            *stream = (userdata->wave_position) *userdata->max_amp * userdata->mixer->master_amp;
            if (userdata->wave_position >= 1) userdata->wave_position = 0;
            break;
        }
        case OSCILLATOR_SQUARE:
        {
            *stream = round(userdata->wave_position) *  userdata->max_amp * userdata->mixer->master_amp;
            if (userdata->wave_position >= 1) userdata->wave_position = 0;
            break;
        }
    }
    userdata->wave_position += userdata->wave_step;
}

void BasicOscillator_init(BasicOscillator *userdata, Mixer *mixer)
{
    userdata->format.wFormatTag = WAVE_FORMAT_PCM;
    userdata->format.nChannels = 1;
    userdata->format.nSamplesPerSec = SAMPLING_RATE;
    userdata->format.wBitsPerSample = 16;
    userdata->format.cbSize = 0;
    userdata->frequency = 400;
    userdata->wave_position = 0;
    userdata->wave_step = 0;
    userdata->amplitude = 0.2f;
    userdata->type = OSCILLATOR_SINE;
    userdata->amp_step = 0.01f;
    userdata->max_amp = (32767 * userdata->amplitude);
    userdata->base_pitch = 48;
    userdata->mixer = mixer;

    wav_init(&userdata->wavdata, BasicOscillator_initStream, 
            BasicOscillator_handleStream, &userdata->format, userdata);

}
void BasicOscillator_play(BasicOscillator *userdata, bool play)
{

    userdata->wavdata.play = play;
}



#endif // _BASIC_OSCILLATOR_H_
