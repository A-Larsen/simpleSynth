#include <stdio.h>

#include "BasicOscillator.h"

bool AUDIO_START = true;
Mixer mixer;

int main(int argc, char **argv)
{

    static bool quit = false;

    BasicOscillator userdata;
    mixer.master_amp = 0.8f;
    BasicOscillator_init(&userdata);

    BasicOscillator_setPitch(&userdata, 72);
    BasicOscillator_play(&userdata, true);

	while(!quit) {
        char key = _getche();

        BasicOscillator_mapKey(&userdata, key);

		switch(key) {
        case 27:  // esc
            quit = true;
            break;
        
        case '1': 
            BasicOscillator_setType(&userdata, OSCILLATOR_SINE); 
            break;

        case '2':
            BasicOscillator_setType(&userdata, OSCILLATOR_SAW); 
            break;

        case '3': 
            BasicOscillator_setType(&userdata, OSCILLATOR_SQUARE);
            break;

        case '-': 
            BasicOscillator_ampDown(&userdata);
            break;

        case '=': 
            BasicOscillator_ampUp(&userdata);
            break;

        case 'z': 
            BasicOscillator_setBasePitch(&userdata, userdata.base_pitch - 12);
            break;

        case 'x': 
            BasicOscillator_setBasePitch(&userdata, userdata.base_pitch + 12); 
            break;
		}
	}

	return 0;
}
