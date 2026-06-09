#include "io.h"

io::Source::Source()
{

}

io::Source::~Source()
{
    
}

io::DeviceSource::DeviceSource(PaDeviceIndex deviceIndex)
{
    this->deviceIndex = deviceIndex;
}

PaDeviceIndex io::DeviceSource::getDeviceIndex()
{
    return this->deviceIndex;
}