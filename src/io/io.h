#pragma once

#include <portaudio.h>


namespace io
{
    class Source
    {
      public:
        Source();
        virtual ~Source();

      private:

    };

    class DeviceSource : public Source
    {
      public:
        DeviceSource(PaDeviceIndex deviceIndex);

        PaDeviceIndex getDeviceIndex();

      private:
        PaDeviceIndex deviceIndex;
        
    };


    class Sink
    {
      public:
        Sink();
        
      private:

    };
}