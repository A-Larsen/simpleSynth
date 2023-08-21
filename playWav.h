#ifndef _PLAY_WAV_H_
#define _PLAY_WAV_H_

#include "audio.h"
typedef struct _WavPlayer {
    float max_amp;
    int16_t *data;
    uint32_t data_len;
    uint32_t pos;
    WAVEFORMATEX *format;
    WavData *wavdata;
    float *master_amp;
} WavPlayer;

void WavPlayer_initStream(WavData *wavdata)
{
}

void WavPlayer_handleStream(int16_t *stream, WavData *wavdata)
{
    WavPlayer *userdata = (WavPlayer *)wavdata->data;
    if(userdata->pos > userdata->data_len) {
        return;
    }
    // multiple this by master amp in the future
    *stream = userdata->data[userdata->pos];
    userdata->pos += 1;

}



#endif // _PLAY_WAV_H_
