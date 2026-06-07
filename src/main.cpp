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
    std::cin.ignore();

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
    std::cin.ignore();

    std::cout << "\033[A\rSelection: " << device_choices.at(choice - 1).second->name << "\n" << std::endl;


    return device_choices.at(choice - 1).first;
}

void ExecuteCommand(std::vector<std::string> tokens)
{
    if (tokens.at(0) == "create")
    {
        if (tokens.at(1) == "source")
        {
            new node::SourceNode(tokens.at(2), UserSelectInputDevice());
        }
        if (tokens.at(1) == "sink")
        {
            new node::SinkNode(tokens.at(2), UserSelectOutputDevice());
        }
    }
    if (tokens.at(0) == "remove")
    {
        if (tokens.at(1) == "node")
        {
            node::Node::remove(tokens.at(2));
        }
        if (tokens.at(1) == "connection")
        {
            node::Stream::remove(node::Node::get<node::SourceNode>(tokens.at(2)), node::Node::get<node::SinkNode>(tokens.at(3)));
        }
    }
    if (tokens.at(0) == "connect" && tokens.at(2) == "to")
    {
        new node::Stream(node::Node::get<node::SourceNode>(tokens.at(1)), node::Node::get<node::SinkNode>(tokens.at(3)));
    }
    if (tokens.at(0) == "start")
    {
        node::Stream::start();
    }
    if (tokens.at(0) == "stop")
    {
        node::Stream::stop();
    }
}

void MainLoop()
{
    std::string user_command;

    do
    {
        std::getline(std::cin, user_command);

        std::vector<std::string> tokens;
        std::string token;
        std::stringstream command_buffer(user_command); 
        while (std::getline(command_buffer, token, ' ')) tokens.push_back(token);

        ExecuteCommand(tokens);
    }
    while (user_command != "quit");
}

int main()
{
    SDL_Init(SDL_INIT_EVENTS);
    Pa_Initialize();

    PrintDependencyVersions();
    PrintDevices();

    MainLoop();

    Pa_Terminate();
    SDL_Quit();
}