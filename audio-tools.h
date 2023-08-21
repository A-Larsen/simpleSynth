#ifndef _AUDIO_TOOLS_H_
#define _AUDIO_TOOLS_H_
#include <math.h>

float pitchToFrequency(float frequency, float semitiones)
{
    return frequency * pow(2, semitiones / 12);
}

#endif // _AUDIO_TOOLS_H_
