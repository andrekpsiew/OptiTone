#include "node.h"
#include "../io/io.h"

node::InputNode::InputNode(std::string nickname, io::Source* source_ptr) : Node(nickname)
{

    /* initializes attributes from provided device information */
    this->source = source_ptr;

    io::DeviceSource* device_source = dynamic_cast<io::DeviceSource*>(source_ptr);
    const PaDeviceInfo* device_info = Pa_GetDeviceInfo(device_source->getDeviceIndex());

    this->total_channels = device_info->maxInputChannels;

    this->sample_rate = device_info->defaultSampleRate;

    this->suggested_low_latency = device_info->defaultLowInputLatency;
    this->suggested_high_latency = device_info->defaultHighInputLatency;

    for (int i = 0; i < this->total_channels; i++)
    {
        this->channel_list.push_back({"channel" + std::to_string(i), i});
    }

    source_node_list.insert({nickname, this});
    printSelf();
}

node::InputNode::~InputNode()
{
    source_node_list.erase(this->getNickname());
}

PaDeviceIndex node::InputNode::getDeviceIndex()
{
    return dynamic_cast<io::DeviceSource*>(this->source)->getDeviceIndex();
}


double node::InputNode::getSampleRate()
{
    return this->sample_rate;
}

short int node::InputNode::getTotalChannels()
{
    return this->total_channels;
}

PaTime node::InputNode::getSuggestedLowLatency()
{
    return this->suggested_low_latency;
}

PaTime node::InputNode::getSuggestedHighLatency()
{
    return this->suggested_high_latency;
}

void node::InputNode::printSelf()
{
    std::cout << "Source node '" << this->getNickname() << "':\n";
    for (int i = 0; i < this->channel_list.size(); i++)
    {
        std::cout << "  " << i << ". '" << channel_list.at(i).first << "'\n";
    }
}