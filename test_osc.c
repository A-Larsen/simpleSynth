#include "BasicOscillator.h"
#include "Oscilloscope.h"
#include "audio.h"

bool AUDIO_START = true;
Mixer mixer;

/* int16_t data = { */
/*     1, */
/*     5583, */
/*     50000, */
/*     32767 */
/* }; */

int main(void)
{
    Oscilloscope scope;


    BasicOscillator userdata;
    mixer.master_amp = 0.8f;
    BasicOscillator_init(&userdata);
    BasicOscillator_init(&userdata);

    BasicOscillator_setPitch(&userdata, 72);
    BasicOscillator_play(&userdata, true);

    SDL_Rect rect = {
        .x = 10, .y = 40,
        .w = 800, .h = 400
    };

    /* int16_t data[0xFFFF]; */
    /* for (int i = 0; i < 0xFFFF; ++i) { */
    /*     data[i] = i - 32767; */
    /* } */

    Oscilloscope_init(&scope, &rect);

    uint32_t data_pos = 0;

    while(1) {
        /* if (data_pos > 0xFFFF) data_pos = 0; */
        /* scope.data = &data[data_pos]; */
        int16_t a = 12;
        scope.data = &a;

        if (Oscilloscope_update(&scope)) break;
        data_pos += 60;;
    }

    Oscilloscope_quit(&scope);
}
