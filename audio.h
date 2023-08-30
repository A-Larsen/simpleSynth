#ifndef _AUDIO_H_ 
#define _AUDIO_H_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <processthreadsapi.h>
#include <mmsystem.h>
#include <stdbool.h>
#include <conio.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "User32.lib")

#include "error.h"

#pragma comment(lib, "winmm.lib")
#define SAMPLING_RATE 44100
#define CHUNK_SIZE 2500
#define CHUNK_COUNT 2

typedef struct _WavData {
    const uint16_t sample_rate;
    HWAVEOUT wave_out;
    WAVEFORMATEX *format;
    const uint16_t chunk_size;
    const uint8_t chunk_count;
    WAVEHDR header[CHUNK_COUNT];
    int16_t chunks[CHUNK_COUNT][CHUNK_SIZE];
    short chunk_swap;
    void (*initStream) (struct _WavData *wavdata);
    void (*handleStream) (int16_t *stream, struct _WavData *wavdata);
    void *data;
    bool play;
} WavData;

extern bool AUDIO_START;


__declspec(dllexport) int wav_init(WavData *wavdata, void (*initStream) (struct _WavData *wavdata), 
            void (*handleStream) (int16_t *stream, struct _WavData *wavdata),
            WAVEFORMATEX *format, void *data);
#endif
