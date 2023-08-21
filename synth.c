#include <stdio.h>

#include "audio.h"
#include "audio-tools.h"

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

bool AUDIO_START = true;

void setStep(UserData *userdata)
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
void initStream(WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
    setStep(userdata);
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
                    stream[i][j] = (userdata->wave_position )  *userdata->max_amp;
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
    WAVEFORMATEX format;
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 1;
    format.nSamplesPerSec = SAMPLING_RATE;
    format.wBitsPerSample = 16;
    format.cbSize = 0;
    UserData userdata = {
        .frequency = 400,
        .wave_position = 0,
        .wave_step = 0,
        .amplitude = 0.2f,
        .type = OSCILLATOR_SINE
    };

    /* if (argc == 4) { */
    /*     char type[8]; */
    /*     sscanf_s(argv[1], "%s", type); */

    /*     if (strcmp(type, "sine") == 0) { */
    /*         userdata.type = OSCILLATOR_SINE; */
    /*     } */
    /*     if (strcmp(type, "saw") == 0) { */
    /*         userdata.type = OSCILLATOR_SAW; */
    /*     } */
    /*     if (strcmp(type, "square") == 0) { */
    /*         userdata.type = OSCILLATOR_SQUARE; */
    /*     } */

    /*     sscanf_s(argv[2], "%f", &userdata.frequency); */
    /*     sscanf_s(argv[3], "%f", &userdata.amplitude); */
    /* } */
    userdata.max_amp = (32767 * userdata.amplitude);
    wav_init(&wavdata, initStream, handleStream, &format, &userdata);
    static bool quit = false;
    float amp_step = 0.01f;
    float freq_step = 10;
    float pitch_step = 1.0f;

    wavdata.play = true;
	while(!quit) {
		switch(_getche()) {
			case 72: { // up
                 if (!(userdata.frequency < (float)SAMPLING_RATE / 2.0f)) {
                     break;
                 }
                userdata.frequency += freq_step;
                setStep(&userdata);
				printf("Frequency: %f\n", userdata.frequency);
                break;
			};
			case 80: { // down
                 if (!(userdata.frequency - freq_step > 0)) {
                     break;
                 }
				userdata.frequency -= freq_step;
                setStep(&userdata);
				printf("Frequency: %f\n", userdata.frequency);
                break;
			}
			case 27: { // esc
				quit = true;
                break;
			} 
			case '1': {
                /* userdata.wave_step = TWOPI / ((float)SAMPLING_RATE / userdata.frequency); */
                userdata.type = OSCILLATOR_SINE;
                setStep(&userdata);
                break;
			}
			case '2': {
                /* userdata.wave_step = 1 / ((float)SAMPLING_RATE / userdata.frequency); */
                userdata.type = OSCILLATOR_SAW;
                setStep(&userdata);
                break;
			}
			case '3': {
                /* userdata.wave_step = 1 / ((float)SAMPLING_RATE / userdata.frequency); */
                userdata.type = OSCILLATOR_SQUARE;
                setStep(&userdata);
                break;
			}
            case 'j': 
            {
                if (userdata.amplitude - amp_step <= 0 ) {
                    break;
                }
                userdata.amplitude -= amp_step;
                userdata.max_amp = (32767 * userdata.amplitude);
                break;
            } 
            case 'k': 
            {
                if (userdata.amplitude + amp_step >= 1 ) {
                    break;
                }
                userdata.amplitude += amp_step;
                userdata.max_amp = (32767 * userdata.amplitude);
                break;
            } 
		}
	}

	return 0;
}
