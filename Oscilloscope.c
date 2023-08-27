#include "Oscilloscope.h"
#include "error.h"
#include <SDL2/SDL_video.h>

static void clear(Oscilloscope *userdata, uint32_t color)
{
    userdata->surface = SDL_GetWindowSurface(userdata->window);
    SDL_Rect screen = {
        .x = 0,
        .y = 0,
        .w = userdata->window_rect.w,
        .h = userdata->window_rect.h,
    };

    SDL_FillRect(userdata->surface, &screen, color);
    SDL_UpdateWindowSurface(userdata->window);

}

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
    clear(userdata, 0x00FF0000);
}

bool Oscilloscope_update(Oscilloscope *userdata)
{
    // fill the background

    // scale the int16 data to the screen width and height
    uint16_t y = ((((float)(*userdata->data + 32767) / (float)0xFFFF) - 1) * -1) * userdata->window_rect.h;
    static uint16_t x = 0;
    if (x == 0) clear(userdata, 0x00FF0000);


    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = 10,
        .h = 10
    };
    SDL_FillRect(userdata->surface, &rect, 0x00000000);
    /* printf("%u\n", yscale); */

    // The xscale is how much time are we going to show on the screen
    int xscale = 0;
    while(SDL_PollEvent(&userdata->event)) {
        switch(userdata->event.type) {
            case SDL_QUIT: 
            {
                return 1;
                break;
            }
            case SDL_KEYDOWN: 
            {
                switch(userdata->event.key.keysym.sym) {
                    case SDLK_q: 
                    {
                        return 1;
                        break;
                    }
                }
                /* printf("nice\n"); */
                break;
            }
        }
    }
    SDL_UpdateWindowSurface(userdata->window);
    x++;
    if (x >= userdata->window_rect.w) {
        x = 0;
    }
    return false;
}

void Oscilloscope_quit(Oscilloscope *userdata)
{
    SDL_Quit();
}
