#include <stdio.h>

#include "BasicOscillator.h"

bool AUDIO_START = true;

int main(int argc, char **argv)
{
    WavData wavdata;
    WAVEFORMATEX format;
    format.wFormatTag = WAVE_FORMAT_PCM;
    format.nChannels = 1;
    format.nSamplesPerSec = SAMPLING_RATE;
    format.wBitsPerSample = 16;
    format.cbSize = 0;
    BasicOscillator userdata = {
        .frequency = 400,
        .wave_position = 0,
        .wave_step = 0,
        .amplitude = 0.2f,
        .type = OSCILLATOR_SINE,
        .amp_step = 0.01f,
        .max_amp = (32767 * userdata.amplitude),
        .base_pitch = 48
    };

    wav_init(&wavdata, BasicOscillator_initStream, BasicOscillator_handleStream, &format, &userdata);
    static bool quit = false;
    wavdata.play = true;
    BasicOscillator_setPitch(&userdata, 72);

	while(!quit) {
        char key = _getche();

        BasicOscillator_mapKey(&userdata, key);

		switch(key) {
			case 27: { // esc
				quit = true;
                break;
			} 
			case '1': BasicOscillator_setType(&userdata, OSCILLATOR_SINE); break;
			case '2': BasicOscillator_setType(&userdata, OSCILLATOR_SAW); break;
			case '3': BasicOscillator_setType(&userdata, OSCILLATOR_SQUARE); break;
            case '-': BasicOscillator_ampDown(&userdata); break;
            case '=': BasicOscillator_ampUp(&userdata); break;
            case 'z': BasicOscillator_setBasePitch(&userdata,
                                                   userdata.base_pitch - 12);
                      break;
            case 'x': BasicOscillator_setBasePitch(&userdata,
                                                   userdata.base_pitch + 12); 
                      break;
		}
	}

	return 0;
}
