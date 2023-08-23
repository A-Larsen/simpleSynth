#ifndef _READWAV_H_
#define _READWAV_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <memory.h>

#ifdef LUALIB
#include <lua/lua.h>
#endif

typedef struct _WavHeader {
    unsigned int cksize; // chunk size
    unsigned short wFormatTag; // format code
    unsigned short nChannels; // number of channels
    unsigned int nSamplesPerSec; // sample rate
    unsigned int nAvgBytesPerSec; // data rate
    unsigned short nBlockALilgn;  // data block size
    unsigned int wBitsPerSample;
    unsigned short cbSize; // Size of the extension: 22
    unsigned short wValidBitsPerSample; // at most 8 * M
    unsigned int dwChannelMask; // Speaker position mask
} WavHeader;

#ifdef LUALIB
__declspec(dllexport) void readWav(const char *file, WavHeader *header, int16_t **data,
             uint32_t *len, lua_State *L);
#else
__declspec(dllexport) void readWav(const char *file, WavHeader *header, int16_t **data,
             uint32_t *len);
#endif

__declspec(dllexport) void freeWavData(int16_t *data);

#endif // _READWAV_H_
