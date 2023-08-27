#include "Oscilloscope.h"
#include "error.h"
#include <SDL2/SDL_video.h>

void Oscilloscope_init(Oscilloscope *userdata, SDL_Rect *rect) {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        PERROR("Could not init sdl", SDL_GetError());
        exit(1);
    }
    memcpy(&userdata->window_rect, rect, sizeof(SDL_Rect));
    userdata->window = SDL_CreateWindow("Oscilloscope", 
                                        rect->x, 
                                        rect->y,
                                        rect->w, 
                                        rect->h, 0);
    userdata->surface = SDL_GetWindowSurface(userdata->window);
}

void Oscilloscope_update(Oscilloscope *userdata)
{
    SDL_Rect screen = {
        .x = 0,
        .y = 0,
        .w = userdata->window_rect.w,
        .h = userdata->window_rect.h,
    };

    SDL_FillRect(userdata->surface, &screen, 0x000000);
    SDL_UpdateWindowSurface(userdata->window);
}
