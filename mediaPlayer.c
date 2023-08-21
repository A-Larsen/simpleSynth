#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "audio.h"
#include "readWav.h"

typedef struct _UserData {
    float max_amp;
    int16_t *data;
    uint32_t data_len;
    uint32_t pos;
    WAVEFORMATEX *format;

} UserData;
bool quit = false;

void initStream(WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
}

void handleStream(int16_t *stream, WavData *wavdata)
{
    UserData *userdata = (UserData *)wavdata->data;
    if(userdata->pos > userdata->data_len) {
        quit = true;
        return;
    }
    *stream = userdata->data[userdata->pos];
    userdata->pos += 1;
}


int main(int argc, char **argv)
{
    WavHeader wavheader;
    WavData wavdata;
    WAVEFORMATEX format;
    UserData userdata;
    userdata.data = NULL;

    readWav("seaShells.wav", &wavheader, &userdata.data, &userdata.data_len);

    format.wFormatTag = wavheader.wFormatTag;
    format.nChannels = wavheader.nChannels;
    format.nSamplesPerSec = wavheader.nSamplesPerSec;
    format.wBitsPerSample = wavheader.wBitsPerSample;
    format.cbSize = wavheader.cbSize;
    userdata.format = &format;
    userdata.pos = 0;

    printf("sampleRate: %lu\n", format.nSamplesPerSec);
    userdata.max_amp = pow(2, format.wBitsPerSample - 1) - 1;
    wav_init(&wavdata, initStream, handleStream, &format, &userdata);
    while(!quit);
    freeWavData(userdata.data);
	return 0;
}
