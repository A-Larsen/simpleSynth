#include "audio.h"
#include "playWav.h"

Mixer mixer;
bool AUDIO_START = true;

int main(int argc, char **argv)
{
    mixer.master_amp = 0.8f;
    WavPlayer wavplayer;
    WavPlayer_init(&wavplayer, "seaShells.wav");
    WavPlayer_play(&wavplayer, true);
    while(1);
    wavPlayer_quit(&wavplayer);
	return 0;
}
