#include "playwav.h"

static void WavPlayer_initStream(WavData *wavdata)
{
}

static void WavPlayer_handleStream(int16_t *stream, WavData *wavdata)
{
    WavPlayer *userdata = (WavPlayer *)wavdata->data;
    if(userdata->pos > userdata->data_len) {
        return;
    }
    *stream = userdata->data[userdata->pos] * mixer.master_amp;
    userdata->pos += 1;

}

// lua state can be null
#ifdef LUALIB
void WavPlayer_init(WavPlayer *userdata, const char *file_path, lua_State *L)
#else
void WavPlayer_init(WavPlayer *userdata, const char *file_path)
#endif
{
    userdata->data = NULL;

    WavHeader wavheader;
#ifdef LUALIB
    readWav(file_path, &wavheader, &userdata->data, &userdata->data_len, L);
#else
    readWav(file_path, &wavheader, &userdata->data, &userdata->data_len);
#endif

    userdata->format.wFormatTag = wavheader.wFormatTag;
    userdata->format.nChannels = wavheader.nChannels;
    userdata->format.nSamplesPerSec = wavheader.nSamplesPerSec;
    userdata->format.wBitsPerSample = wavheader.wBitsPerSample;
    userdata->format.cbSize = wavheader.cbSize;
    userdata->pos = 0;
    userdata->max_amp = pow(2, userdata->format.wBitsPerSample - 1) - 1;

    wav_init(&userdata->wavdata, WavPlayer_initStream, WavPlayer_handleStream,
            &userdata->format, userdata);
    
}

void WavPlayer_play(WavPlayer *userdata, bool play)
{
    userdata->wavdata.play = play;
}

void wavPlayer_quit(WavPlayer *userdata)
{
    freeWavData(userdata->wavdata.data);
}

