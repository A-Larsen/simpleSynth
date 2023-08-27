#ifndef _OSCILLOSCOPE_H_
#define _OSCILLOSCOPE_H_
#include "audio.h"
#include "mixer.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#define SDL_main main

typedef struct _Oscilloscope {
    SDL_Window *window;
    SDL_Event event;
    SDL_Surface *surface;
    SDL_Rect window_rect;
    int16_t *data;
} Oscilloscope;

__declspec(dllexport) void Oscilloscope_init(Oscilloscope *userdata, SDL_Rect *rect);
__declspec(dllexport) bool Oscilloscope_update(Oscilloscope *userdata, uint16_t speed);
__declspec(dllexport) void Oscilloscope_quit(Oscilloscope *userdata);

#endif // _OSCILLOSCOPE_H_
