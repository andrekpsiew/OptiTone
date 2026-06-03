#include <SDL2/SDL.h>
#include <portaudio.h>

#include "chain.h"

#include <iostream>

void PrintDependencyVersions()
{
    std::cout << "\n\n";

    SDL_version version;
    SDL_GetVersion(&version);
    std::cout 
        << "SDL2 v" 
        << (int)version.major 
        << "." 
        << (int)version.minor 
        << "." 
        << (int)version.patch
        << "\n";

    std::cout 
        << Pa_GetVersionText() 
        << "\n";
    
    std::cout << "\n" << std::endl;
}

void PrintDevices()
{
    int input_devices = 0;
    int output_devices = 0;
    int full_duplex_devices = 0;

    for (int idx = 0; idx < Pa_GetDeviceCount(); idx++)
    {
        const PaDeviceInfo* this_device = Pa_GetDeviceInfo(idx);

        if (this_device->maxInputChannels > 0 && this_device->maxOutputChannels > 0)
        {
            full_duplex_devices++;
        }
        else if (this_device->maxInputChannels > 0)
        {
            input_devices++;
        }
        else if (this_device->maxOutputChannels > 0)
        {
            output_devices++;
        }
    }

    std::cout 
        << "Total devices: " 
        << Pa_GetDeviceCount()
        << "\n"
        << "     Input devices: "
        << input_devices
        << "\n"
        << "    Output devices: "
        << output_devices
        << "\n";

    if (full_duplex_devices > 0)
    {
        std::cout
            << "       Full-duplex: "
            << full_duplex_devices
            << "\n";
    }

    for (int i = 0; i < Pa_GetDeviceCount(); i++)
    {
        std::cout 
            << "\n  "
            << i + 1 
            << ". " 
            << Pa_GetDeviceInfo(i)->name
            << "\n       Input channels: "
            << Pa_GetDeviceInfo(i)->maxInputChannels
            << "\n      Output channels: "
            << Pa_GetDeviceInfo(i)->maxOutputChannels
            << "\n  Default sample-rate: "
            << Pa_GetDeviceInfo(i)->defaultSampleRate
            << "hz"
            << "\n";
    }

    std::cout << "\n" << std::endl;
}

int main()
{
    SDL_Init(SDL_INIT_EVENTS);
    Pa_Initialize();

    PrintDependencyVersions();
    PrintDevices();

    

 
    Pa_Terminate();
    SDL_Quit();
}