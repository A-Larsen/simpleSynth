#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "audio.h"
#include "readWav.h"

typedef struct _UserData {
    float max_amp;
    int16_t *data;
    uint32_t data_len;
    float frequency;
    float wave_step;
    float wave_position;
    WAVEFORMATEX *format;

} UserData;

void initStream(WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
}

void handleStream(int16_t *stream, WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
    if(userdata->wave_position > userdata->data_len) {
        return;
    }
    *stream = *userdata->data;
    /* *stream = userdata->data[(int)userdata->wave_position]; */
    /* memcpy(stream, userdata->data, 1); */
    /* userdata->wave_position += userdata->wave_step; */
    userdata->data += (int)userdata->frequency;
}


int main(int argc, char **argv)
{
    WavHeader wavheader;
    WavData wavdata;
    WAVEFORMATEX format;
    UserData userdata;
    userdata.data = NULL;

    readWav("test.wav", &wavheader, &userdata.data, &userdata.data_len);
    /* format.wFormatTag = WAVE_FORMAT_PCM; */
    /* format.nChannels = 1; */
    /* format.nSamplesPerSec = SAMPLING_RATE; */
    /* format.wBitsPerSample = 16; */
    /* format.cbSize = 0; */
    /* printf("data_len: %d\n", userdata.data_len); */
    /* for (uint32_t i = 0; i < userdata.data_len; ++i) { */
    /*     printf("%d\n", userdata.data[i]); */
    /* } */

    format.wFormatTag = wavheader.wFormatTag;
    format.nChannels = wavheader.nChannels;
    format.nSamplesPerSec = wavheader.nSamplesPerSec;
    format.wBitsPerSample = wavheader.wBitsPerSample;
    format.cbSize = wavheader.cbSize;
    userdata.format = &format;

    printf("sampleRate: %lu\n", format.nSamplesPerSec);
    userdata.max_amp = pow(2, format.wBitsPerSample - 1) - 1;
    userdata.frequency = 1;
    userdata.wave_position = 0;
    userdata.wave_step = 1.0f / ((float)format.nSamplesPerSec / (float)userdata.frequency);
    printf("wave_step: %f\n", userdata.wave_step);
    wav_init(&wavdata, initStream, handleStream, &format, &userdata);
    while(1);
    /* while(1){ printf("nice\n"); }; */
    freeWavData(userdata.data);
	return 0;
}
