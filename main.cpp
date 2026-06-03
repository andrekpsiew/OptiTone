#include <portaudio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

typedef struct {
    double phase;
    double phaseIncrement;
} SynthState;

static int audioCallback(
    const void* inputBuffer,
    void* outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void* userData)
{
    SynthState* synth = (SynthState*)userData;
    float* out = (float*)outputBuffer;

    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        float sample = (sinf(2.0f * M_PI * synth->phase) + sinf(2.0f * M_PI * 2 * synth->phase)) / 2;

        synth->phase += synth->phaseIncrement;
        if (synth->phase >= 1.0) synth->phase -= 1.0;

        *out++ = sample;    // left
        *out++ = sample;    // right
    }

    return paContinue;
}

int main() {
    float HZ = 440;

    SynthState synth;
    synth.phase = 0.0;
    synth.phaseIncrement = HZ / SAMPLE_RATE;

    Pa_Initialize();

    PaStream* stream;
    Pa_OpenDefaultStream(
        &stream,
        0,              // no input channels
        2,              // stereo output
        paFloat32,      // sample format
        SAMPLE_RATE,
        FRAMES_PER_BUFFER,
        audioCallback,
        &synth
    );

    Pa_StartStream(stream);

    Pa_Sleep(3000);     // play for 3 seconds

    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();

    return 0;
}