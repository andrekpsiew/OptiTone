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
        << std::endl;

    std::cout 
        << Pa_GetVersionText() 
        << "\n"
        << std::endl;
}

int main()
{
    SDL_Init(SDL_INIT_EVENTS);
    Pa_Initialize();

    PrintDependencyVersions();

    
 
    Pa_Terminate();
    SDL_Quit();
}