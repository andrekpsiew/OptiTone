#include <SDL2/SDL.h>
#include <portaudio.h>

#include "chain.h"

#include <iostream>
#include <vector>

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

std::vector<PaDeviceInfo> LoadDevices()
{
    std::vector<PaDeviceInfo> device_list;

    for (int idx = 0; idx < Pa_GetDeviceCount(); idx++)
    {
        device_list.push_back(*Pa_GetDeviceInfo(idx));
    }

    return device_list;
}

PaDeviceInfo UserSelectInputDevice(std::vector<PaDeviceInfo> device_list)
{
    std::vector<PaDeviceInfo> options;

    for (int idx = 0; idx < device_list.size(); idx++)
    {
        PaDeviceInfo this_device = device_list.at(idx);

        if (this_device.maxInputChannels > 0)
        {
            options.push_back(this_device);
        }
    }

    std::cout << "Select an input device:\n";

    for (int idx = 0; idx < options.size(); idx++)
    {
        std::cout
            << "(" 
            << idx + 1 
            << ") "
            << options.at(idx).name
            << "\n";
    }
    
    std::cout << "\n" << std::endl;

    int choice = -1;
    while (choice > options.size() || choice < 1)
    {
        std::cout << "\033[A\033[2K\rSelection: ";
        std::cin >> choice;
    }
    std::cout 
        << "\033[A\rSelection: "
        << options.at(choice - 1).name
        << "\n\n";

    return options.at(choice - 1);
}

PaDeviceInfo UserSelectOutputDevice(std::vector<PaDeviceInfo> device_list)
{
    std::vector<PaDeviceInfo> options;

    for (int idx = 0; idx < device_list.size(); idx++)
    {
        PaDeviceInfo this_device = device_list.at(idx);

        if (this_device.maxOutputChannels > 0)
        {
            options.push_back(this_device);
        }
    }

    std::cout << "Select an output device:\n";

    for (int idx = 0; idx < options.size(); idx++)
    {
        std::cout
            << "(" 
            << idx + 1 
            << ") "
            << options.at(idx).name
            << "\n";
    }
    
    std::cout << "\n" << std::endl;

    int choice = -1;
    while (choice > options.size() || choice < 1)
    {
        std::cout << "\033[A\033[2K\rSelection: ";
        std::cin >> choice;
    }
        std::cout 
        << "\033[A\rSelection: "
        << options.at(choice - 1).name
        << "\n\n";

    return options.at(choice - 1);
}


int main()
{
    SDL_Init(SDL_INIT_EVENTS);
    Pa_Initialize();

    PrintDependencyVersions();
    PrintDevices();

    std::vector<PaDeviceInfo> device_list = LoadDevices();

    PaDeviceInfo chosen_input_device = UserSelectInputDevice(device_list);
    PaDeviceInfo chosen_output_device = UserSelectOutputDevice(device_list);

 
    Pa_Terminate();
    SDL_Quit();
}