#include <SDL2/SDL.h>
#include <portaudio.h>

#include "chain.h"

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

PaStream* ProvideStream(int input_device_i, int output_device_i)
{
    const PaDeviceInfo* input_device = Pa_GetDeviceInfo(input_device_i);
    const PaDeviceInfo* output_device = Pa_GetDeviceInfo(output_device_i);

    PaStreamParameters input_device_parameters = 
    {
        input_device_i,
        input_device->maxInputChannels,
        paFloat32,
        input_device->defaultLowInputLatency,
        NULL
    };

    PaStreamParameters output_device_parameters = 
    {
        output_device_i,
        output_device->maxOutputChannels,
        paFloat32,
        output_device->defaultLowOutputLatency,
        NULL
    };


    PaError format_support = Pa_IsFormatSupported(&input_device_parameters, &output_device_parameters, 48000.0);
    
    if (format_support != paFormatIsSupported)
    {
        std::cout << "ERROR: " << Pa_GetErrorText(format_support) << std::endl;
        return NULL;
    }
    std::cout << "Stream between " << input_device->name << " and " << output_device->name << " is supported" << std::endl;

    
    PaStream* stream = nullptr;
    
    if 
    (
        Pa_OpenStream(
            &stream,
            &input_device_parameters,
            &output_device_parameters,
            48000.0,
            256,
            paNoFlag,
            audioCallback,
            NULL
        ) != paNoError
    )
    {
        std::cout << "ERROR: " << Pa_GetErrorText(format_support) << std::endl;
        return NULL;
    }

    std::cout << "Opened stream between " << input_device->name << " and " << output_device->name << '\n' << std::endl;
    return stream;
}

std::pair<std::string, PaStream*> UserEstablishStream()
{
    PaDeviceIndex input_device_i = UserSelectInputDevice();
    PaDeviceIndex output_device_i = UserSelectOutputDevice();

    PaStream* stream = ProvideStream(input_device_i, output_device_i);

    std::cout << "\nName this stream: ";
    
    std::string identifier;
    if (std::cin.peek() == '\n') std::cin.ignore();
    std::getline(std::cin, identifier);
    std::cin >> identifier;
    std::cout << "\n";

    return {identifier, stream};
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

    std::vector<std::pair<std::string, PaStream*>> streams;

    std::string user_command;
    do
    {
        std::cout << "\033[2K\r:";
        if (std::cin.peek() == '\n') std::cin.ignore();
        std::getline(std::cin, user_command);


        std::stringstream string_stream(user_command);
        std::string token;
        std::vector<std::string> command_tokens;

        while (std::getline(string_stream, token, ' ')) {
            command_tokens.push_back(token);
        }


        if (command_tokens.at(0) == "new")
        {
            if (command_tokens.at(1) == "stream")
            {
                std::pair<std::string, PaStream*> new_stream = UserEstablishStream();
                if (new_stream.second != NULL)
                {
                    streams.push_back(new_stream);
                }
            }
        }
        else if (command_tokens.at(0) == "start")
        {
            for (int i = 0; i < streams.size(); ++i)
            {
                Pa_StartStream(streams.at(i).second);
            }
        }
        else if (command_tokens.at(0) == "stop")
        {
            for (int i = 0; i < streams.size(); ++i)
            {
                Pa_StopStream(streams.at(i).second);
            }
        }
        else if (command_tokens.at(0) == "kill")
        {
            UserKillStream(streams);
        }
        else if (command_tokens.at(0) == "set")
        {
            if (command_tokens.at(1) == "gain")
            {
                
            }
        }
    } 
    while (user_command != "quit");
    


    Pa_Terminate();
    SDL_Quit();
}