#include "audioProc.h"

#include <raylib.h>

#include <cmath>

#define AMPL_HIST_COUNT 100

static float amplitudeHistory[AMPL_HIST_COUNT] = {0.0f};

static void AudioProcIter(void *buffer, unsigned int frames) {
    float *samples = (float *)buffer;
    float average = 0.0f;

    for (unsigned int frame = 0; frame < frames; frame++) {
        float *left = &samples[frame * 2 + 0], *right = &samples[frame * 2 + 1];

        average += fabsf(*left) / frames;
        average += fabsf(*right) / frames;
    }

    for (int i = 0; i < AMPL_HIST_COUNT - 1; i++) amplitudeHistory[i] = amplitudeHistory[i + 1];

    amplitudeHistory[AMPL_HIST_COUNT - 1] = average;
}

float *synthrush::GetAmplitudes() { return amplitudeHistory; }

void synthrush::InitAudioProcessor() { AttachAudioMixedProcessor(AudioProcIter); }

void synthrush::DeinitAudioProcessor() { DetachAudioMixedProcessor(AudioProcIter); }
