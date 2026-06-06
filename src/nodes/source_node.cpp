#include "node.h"

node::SourceNode::SourceNode(std::string nickname, PaDeviceIndex device_index) : Node(nickname)
{
    const PaDeviceInfo* device_info = Pa_GetDeviceInfo(device_index);

    /* initializes attributes from provided device information */
    this->device_index = device_index;

    this->total_channels = device_info->maxInputChannels;

    this->sample_rate = device_info->defaultSampleRate;

    this->suggested_low_latency = device_info->defaultLowInputLatency;
    this->suggested_high_latency = device_info->defaultHighInputLatency;


    source_node_list.insert({nickname, this});
}

node::SourceNode::~SourceNode()
{
    source_node_list.erase(this->getNickname());
}


double node::SourceNode::getSampleRate()
{
    return this->sample_rate;
}

short int node::SourceNode::getTotalChannels()
{
    return this->total_channels;
}

PaTime node::SourceNode::getSuggestedLowLatency()
{
    return this->suggested_low_latency;
}

PaTime node::SourceNode::getSuggestedHighLatency()
{
    return this->suggested_high_latency;
}

PaDeviceIndex node::SourceNode::getDeviceIndex()
{
    return this->device_index;
}
