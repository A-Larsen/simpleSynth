#ifndef _AUDIO_TOOLS_H_
#define _AUDIO_TOOLS_H_
#include <math.h>

float pitchToFrequency(float semitone)
{
        semitone -= 4 * 12;
        // 440 hz is A4
        return 440 * pow(2, semitone / 12);
}
float frequencyToPitch(float p)
{
    /* return log(p / 261.6262) / */ 
}

#endif // _AUDIO_TOOLS_H_
