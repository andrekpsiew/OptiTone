#include "node.h"

node::SinkNode::SinkNode(std::string nickname, PaDeviceIndex device_index) : Node(nickname)
{
    const PaDeviceInfo* device_info = Pa_GetDeviceInfo(device_index);

    /* initializes attributes from provided device information */
    this->device_index = device_index;

    this->total_channels = device_info->maxOutputChannels;

    this->sample_rate = device_info->defaultSampleRate;

    this->suggested_low_latency = device_info->defaultLowOutputLatency;
    this->suggested_high_latency = device_info->defaultHighOutputLatency;


    sink_node_list.insert({nickname, this});
}

node::SinkNode::~SinkNode()
{
    sink_node_list.erase(this->getNickname());
}


double node::SinkNode::getSampleRate()
{
    return this->sample_rate;
}

short int node::SinkNode::getTotalChannels()
{
    return this->total_channels;
}

PaTime node::SinkNode::getSuggestedLowLatency()
{
    return this->suggested_low_latency;
}

PaTime node::SinkNode::getSuggestedHighLatency()
{
    return this->suggested_high_latency;
}

PaDeviceIndex node::SinkNode::getDeviceIndex()
{
    return this->device_index;
}
