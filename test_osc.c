#include "Oscilloscope.h"
#include "audio.h"

bool AUDIO_START = true;
Mixer mixer;

int main(void)
{
    Oscilloscope scope;

    SDL_Rect rect = {
        .x = 10, .y = 40,
        .w = 800, .h = 400
    };

    Oscilloscope_init(&scope, &rect);

    while(1) {
        if (Oscilloscope_update(&scope)) break;
        SDL_Delay(10);
    }

    Oscilloscope_quit(&scope);
}
