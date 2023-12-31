#include "audio-tools.h"

float pitchToFrequency(uint16_t p)
{
    return pow(2, (((float)p - 60.0f) / 12.0f)) * MIDDLE_C;
}

uint16_t frequencyToPitch(float f)
{
    // C0 = pitch of 12
    // c4 = pitch of 60
    // middle c = c4 = 261.626

    // round(12 * log(f / 261.626) / log(2)) gives us the half steps awawy from
    // middle c
    return 60 + round(12 * log(f / MIDDLE_C) / log(2));
}
