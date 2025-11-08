#include "audioProc.h"

#include <raylib.h>

#include <cmath>

#define AMPL_HIST_COUNT 25

static float amplitudeHistoryL[AMPL_HIST_COUNT] = {0.0f};
static float amplitudeHistoryR[AMPL_HIST_COUNT] = {0.0f};

static void AudioProcIter(void *buffer, unsigned int frames) {
    float *samples = (float *)buffer;
    float averageL = 0.0f;
    float averageR = 0.0f;

    for (unsigned int frame = 0; frame < frames; frame++) {
        float *left = &samples[frame * 2 + 0], *right = &samples[frame * 2 + 1];

        averageL += fabsf(*left) / frames;
        averageR += fabsf(*right) / frames;
    }

    for (int i = 0; i < AMPL_HIST_COUNT - 1; i++) {
        amplitudeHistoryR[i] = amplitudeHistoryR[i + 1];
        amplitudeHistoryL[i] = amplitudeHistoryL[i + 1];
    }

    amplitudeHistoryR[AMPL_HIST_COUNT - 1] = averageR;
    amplitudeHistoryL[AMPL_HIST_COUNT - 1] = averageL;
}

float *synthrush::GetAmplitudesL() { return amplitudeHistoryL; }
float *synthrush::GetAmplitudesR() { return amplitudeHistoryR; }

void synthrush::InitAudioProcessor() { AttachAudioMixedProcessor(AudioProcIter); }

void synthrush::DeinitAudioProcessor() { DetachAudioMixedProcessor(AudioProcIter); }
