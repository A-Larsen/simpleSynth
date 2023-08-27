#include "Oscilloscope.h"
#include "error.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

static void setColor(Oscilloscope *userdata, uint32_t color)
{
    SDL_SetRenderDrawColor(userdata->renderer,(color >> 16) & 0xFF, 
                                    (color >> 8) & 0xFF, color & 0xFF, 
                                    (color >> 24) & 0xFF);

}

static void clear(Oscilloscope *userdata, uint32_t color)
{
    userdata->surface = SDL_GetWindowSurface(userdata->window);
    SDL_Rect screen = {
        .x = 0,
        .y = 0,
        .w = userdata->window_rect.w,
        .h = userdata->window_rect.h,
    };

    setColor(userdata, color);
    SDL_RenderClear(userdata->renderer);
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
    userdata->renderer = SDL_CreateRenderer(userdata->window, -1, 
                                            SDL_RENDERER_SOFTWARE);
    userdata->speed = 1;
}

void Oscilloscope_changeSpeed(Oscilloscope *userdata, uint16_t speed)
{
    userdata->speed = speed;
}

bool Oscilloscope_update(Oscilloscope *userdata)
{
    // fill the background

    // scale the int16 data to the screen width and height
    uint16_t y = ((((float)(*userdata->data + 32767) / 
                    (float)0xFFFF) - 1) * -1) * userdata->window_rect.h;
    static uint16_t x = 0;
    static uint16_t prev_x = 0;
    static uint16_t prev_y = 0;
    if (x == 0) clear(userdata, 0x00FF0000);


    int width = 10;
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = width,
        .h = width
    };
    setColor(userdata, 0x00000000);
    SDL_RenderFillRect(userdata->renderer, &rect);
    SDL_RenderDrawLine(userdata->renderer, prev_x, prev_y + width / 2, x, y + width/ 2);

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
                    case SDLK_EQUALS: 
                    {
                        userdata->speed++;
                        printf("plus\n");
                        break;
                    }
                    case SDLK_MINUS: 
                    {
                        userdata->speed--;
                        printf("plus\n");
                        break;
                    }
                }
                /* printf("nice\n"); */
                break;
            }
        }
    }

    SDL_RenderPresent(userdata->renderer);
    prev_x = x;
    prev_y = y;
    x += userdata->speed;
    if (x >= userdata->window_rect.w) {
        prev_x = 0;
        x = 0;
    }
    return false;
}

void Oscilloscope_quit(Oscilloscope *userdata)
{
    SDL_DestroyRenderer(userdata->renderer);
    SDL_Quit();
}
