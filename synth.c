#include <stdio.h>

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


void initStream(WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
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

void handleStream(int16_t *stream, WavData *wavdata)
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

void fillBuffs(UserData *userdata, int16_t stream[3][SAMPLING_RATE])
{
    /* int16_t stream = 0; */
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < SAMPLING_RATE; ++j) {
            switch(i) {
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
            switch(i) {
                case OSCILLATOR_SINE:
                {
                    stream[i][j] = sin(userdata->wave_position) * userdata->max_amp;
                    if (userdata->wave_position >= TWOPI) userdata->wave_position = 0;
                    break;
                }
                case OSCILLATOR_SAW:
                {
                    stream[i][j] = ((userdata->wave_position *  2) - 1) *userdata->max_amp;
                    if (userdata->wave_position >= 1) userdata->wave_position = 0;
                    break;
                }
                case OSCILLATOR_SQUARE:
                {
                    stream[i][j] = round(userdata->wave_position) *  userdata->max_amp;
                    if (userdata->wave_position >= 1) userdata->wave_position = 0;
                    break;
                }
            }
        }
    }

}

int main(int argc, char **argv)
{
    WavData wavdata;
    UserData userdata = {
        .frequency = 400,
        .wave_position = 0,
        .wave_step = 0,
        .amplitude = 0.2f
    };

    if (argc == 4) {
        char type[8];
        sscanf_s(argv[1], "%s", type);

        if (strcmp(type, "sine") == 0) {
            userdata.type = OSCILLATOR_SINE;
        }
        if (strcmp(type, "saw") == 0) {
            userdata.type = OSCILLATOR_SAW;
        }
        if (strcmp(type, "square") == 0) {
            userdata.type = OSCILLATOR_SQUARE;
        }

        sscanf_s(argv[2], "%f", &userdata.frequency);
        sscanf_s(argv[3], "%f", &userdata.amplitude);
    }
    userdata.max_amp = (32767 * userdata.amplitude);
    wav_init(&wavdata, initStream, handleStream, &userdata);
}
