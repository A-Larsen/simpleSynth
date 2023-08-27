#include "readwav.h"
#include "error.h"

static bool isFmt(char *p)
{
    char a[] = "\x66\x6d\x74\x20";
    for (int i = 0; i < 4; ++i) {
        if (a[i] != p[i]) return false;
    }
    return true;
}

static bool isData(char *p)
{
    char a[] = "data";
    for (int i = 0; i < 4; ++i) {
        if (a[i] != p[i]) return false;
    }
    return true;
}

#ifdef LUALIB
void readWav(const char *file, WavHeader *header, int16_t **data,
             uint32_t *len, lua_State *L)
#else
void readWav(const char *file, WavHeader *header, int16_t **data,
             uint32_t *len)
#endif
{
    FILE *fp = NULL;

    // this will not work correctly if writing mode is not if binary mode "wb"
    if (fopen_s(&fp, file, "rb") != 0) {
        PERROR("could not open WAV file");
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    unsigned int file_len = ftell(fp);
    rewind(fp);

    for(unsigned int i = 0; i < file_len; ++i) {
        fseek(fp, i, SEEK_SET);
        char p[4];
        fread(p, 4, 1, fp);
        if (isFmt(p)) break;

    }
    
    fread(&header->cksize, 4, 1, fp); 
    fread(&header->wFormatTag, 2, 1, fp);
    fread(&header->nChannels, 2, 1, fp);
    fread(&header->nSamplesPerSec, 4, 1, fp);
    fread(&header->nAvgBytesPerSec, 4, 1, fp);
    fread(&header->nBlockALilgn, 2, 1, fp);
    fread(&header->wBitsPerSample, 2, 1, fp);
    fread(&header->cbSize, 2, 1, fp);
    fread(&header->wValidBitsPerSample, 2, 1, fp); 
    fread(&header->dwChannelMask, 2, 1, fp);

    for(unsigned int i = 0; i < file_len; ++i) {
        fseek(fp, i, SEEK_SET);
        char p[4];
        fread(p, 4, 1, fp);
        if (isData(p)) {
            fread(len, 4, 1, fp);
            break;
        }

    }

#ifdef LUALIB
        *data = (int16_t * )lua_newuserdata(L, sizeof(int16_t) * (*len));
#else
        *data = (int16_t * )malloc(sizeof(int16_t) * (*len));
#endif

    int read = fread(*data, *len, 1, fp);
    fclose(fp);
}

void freeWavData(int16_t *data)
{
    free(data);
}
