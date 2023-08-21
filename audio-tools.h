#ifndef _AUDIO_TOOLS_H_
#define _AUDIO_TOOLS_H_
#include <math.h>
#include <stdint.h>

float pitchToFrequency(float semitone)
{
        semitone -= 4 * 12;
        // 440 hz is A4
        return 440 * pow(2, semitone / 12);
}
uint16_t frequencyToPitch(float f)
{
    // C0 = pitch of 12
    // c4 = pitch of 60
    // middle c = c4 = 261.626

    // round(12 * log(f / 261.626) / log(2)) gives us the half steps awawy from
    // middle c
    return 60 + round(12 * log(f / 261.626) / log(2));
}

#endif // _AUDIO_TOOLS_H_
