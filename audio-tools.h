#ifndef _AUDIO_TOOLS_H_
#define _AUDIO_TOOLS_H_
#include <math.h>
#include <stdint.h>

#define MIDDLE_C 261.626f

__declspec(dllexport) float pitchToFrequency(uint16_t p);
__declspec(dllexport) uint16_t frequencyToPitch(float f);

#endif // _AUDIO_TOOLS_H_
