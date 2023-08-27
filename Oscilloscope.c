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

bool Oscilloscope_update(Oscilloscope *userdata)
{
    SDL_Rect screen = {
        .x = 0,
        .y = 0,
        .w = userdata->window_rect.w,
        .h = userdata->window_rect.h,
    };

    SDL_FillRect(userdata->surface, &screen, 0x00FF0000);
    SDL_UpdateWindowSurface(userdata->window);
    while(SDL_PollEvent(&userdata->event)) {
        switch(userdata->event.type) {
            case SDL_QUIT: 
            {
                
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
                printf("nice\n");
                break;
            }
        }
    }
    return false;
}

void Oscilloscope_quit(Oscilloscope *userdata)
{
    SDL_Quit();
}
