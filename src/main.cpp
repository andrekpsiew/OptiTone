#include <SDL2/SDL.h>
#include <portaudio.h>

#include "nodes/node.h"
#include "stream.h"

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <utility>

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

PaDeviceIndex UserSelectInputDevice()
{
    std::vector<std::pair<PaDeviceIndex, const PaDeviceInfo*>> device_choices;

    for (int i = 0; i < Pa_GetDeviceCount(); ++i)
    {
        const PaDeviceInfo* this_device = Pa_GetDeviceInfo(i);

        if (this_device->maxInputChannels > 0)
        {
            device_choices.push_back({i, this_device});
        }
    }

    std::cout << "Select an input device:\n";
    for (int i = 0; i < device_choices.size(); i++)
    {
        std::cout << "(" << i + 1 << ") " << device_choices.at(i).second->name << "\n";
    }
    std::cout << "\n" << std::endl;

    int choice = -1;
    while (choice > device_choices.size() || choice < 1)
    {
        std::cout << "\033[A\033[2K\rSelection: ";
        std::cin >> choice;
    }

    std::cout << "\033[A\rSelection: " << device_choices.at(choice - 1).second->name << "\n" << std::endl;


    return device_choices.at(choice - 1).first;
}

PaDeviceIndex UserSelectOutputDevice()
{
    std::vector<std::pair<PaDeviceIndex, const PaDeviceInfo*>> device_choices;

    for (int i = 0; i < Pa_GetDeviceCount(); ++i)
    {
        const PaDeviceInfo* this_device = Pa_GetDeviceInfo(i);

        if (this_device->maxOutputChannels > 0)
        {
            device_choices.push_back({i, this_device});
        }
    }

    std::cout << "Select an output device:\n";
    for (int i = 0; i < device_choices.size(); i++)
    {
        std::cout << "(" << i + 1 << ") " << device_choices.at(i).second->name << "\n";
    }
    std::cout << "\n" << std::endl;

    int choice = -1;
    while (choice > device_choices.size() || choice < 1)
    {
        std::cout << "\033[A\033[2K\rSelection: ";
        std::cin >> choice;
    }

    std::cout << "\033[A\rSelection: " << device_choices.at(choice - 1).second->name << "\n" << std::endl;


    return device_choices.at(choice - 1).first;
}

void UserKillStream(std::vector<std::pair<std::string, PaStream*>> streams)
{
    std::cout << "Select a stream to kill:\n";
    for (int i = 0; i < streams.size(); ++i)
    {
        std::cout << "(" << i + 1 << ") " << streams.at(i).first << "\n";
    }
    std::cout << std::endl;

    int choice = -1;
    while (choice > streams.size() || choice < 1)
    {
        std::cout << "\033[A\033[2K\rSelection: ";
        std::cin >> choice;
    }

    std::cout << "\033[A\rSelection: " << streams.at(choice - 1).first << "\n" << std::endl;

    Pa_CloseStream(streams.at(choice - 1).second);
    streams.erase(streams.begin() + (choice - 1));
}


int main()
{
    SDL_Init(SDL_INIT_EVENTS);
    Pa_Initialize();

    PrintDependencyVersions();
    PrintDevices();
    
    PaDeviceIndex input = UserSelectInputDevice();
    new node::SourceNode("mic", input);

    PaDeviceIndex output = UserSelectOutputDevice();
    new node::SinkNode("earbuds", output);

    new node::Stream(node::Node::get<node::SourceNode>("mic"), node::Node::get<node::SinkNode>("earbuds"));

    node::Stream::start();
    Pa_Sleep(4000);
    node::Stream::stop();

    Pa_Terminate();
    SDL_Quit();
}