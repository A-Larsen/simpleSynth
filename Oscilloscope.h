#ifndef _OSCILLOSCOPE_H_
#define _OSCILLOSCOPE_H_
#include "audio.h"
#include <SDL2/SDL.h>

typedef struct _Oscilloscope {
    SDL_Window *window;
    SDL_Event event;
    SDL_Surface *surface;
    SDL_Rect window_rect;
} Oscilloscope;

#endif // _OSCILLOSCOPE_H_
