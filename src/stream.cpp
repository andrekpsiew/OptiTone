#include "stream.h"

#include <iostream>

int audioCallback(
    const void *input,
    void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    const float GAIN = 3.0f;

    const float* in = static_cast<const float*>(input);
    float* out = static_cast<float*>(output);

    if (!in)
        return paContinue;

    for (unsigned long i = 0; i < frameCount; i++)
    {
        float s = in[i] * GAIN;

        out[i * 2]     = s; // left
        out[i * 2 + 1] = s; // right
    }

    return paContinue;
}


node::Stream::Stream(node::SourceNode* source, node::SinkNode* sink)
{
    /* generate parameters from provided nodes */

    PaStreamParameters input_node_parameters = 
    {
        source->getDeviceIndex(),
        source->getTotalChannels(),
        paFloat32,
        source->getSuggestedLowLatency(),
        NULL
    };

    PaStreamParameters output_node_parameters = 
    {
        sink->getDeviceIndex(),
        sink->getTotalChannels(),
        paFloat32,
        sink->getSuggestedLowLatency(),
        NULL
    };


    /* check if opening stream is possible */

    PaError format_support = Pa_IsFormatSupported(&input_node_parameters, &output_node_parameters, 48000.0);

    if (format_support != paFormatIsSupported)
    {
        std::cout << "ERROR: " << Pa_GetErrorText(format_support) << std::endl;
        // error
    }

    std::cout << "Stream between " << source->getNickname() << " and " << sink->getNickname() << " is supported" << std::endl;

    
    /* open the stream */

    PaStream* stream = nullptr;

    if 
    (
        Pa_OpenStream(
            &stream,
            &input_node_parameters,
            &output_node_parameters,
            48000.0,
            256,
            paNoFlag,
            audioCallback,
            NULL
        ) != paNoError
    )
    {
        std::cout << "ERROR: " << Pa_GetErrorText(format_support) << std::endl;
        // error
    }

    std::cout << "Opened stream between " << source->getNickname() << " and " << sink->getNickname() << '\n' << std::endl;
    
    // somehow return the stream
    Pa_StartStream(stream);
}
