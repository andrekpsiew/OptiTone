#pragma once

#include <portaudio.h>
#include <unordered_map>
#include <vector>
#include <string>


namespace node
{
    /* only for managing all Node inherited objects and as the parent constructor for them */
    class Node
    {
      public:
        Node(std::string nickname);
        virtual ~Node();

        std::string getNickname();

        /* returns a node found in the hashmap by its true type */
        template <typename NodeType> static NodeType* get(std::string nickname)
        {
            auto node = node_list.find(nickname);
            if (node == node_list.end()) return nullptr;
            return dynamic_cast<NodeType*>(node->second);
        }

      private:
        /* list containing every Node inherited child object regardless of its specialization */
        inline static   std::unordered_map<std::string, Node*> node_list;

        /* attributes of every Node inherited child object */
        std::string     nickname;
    };


    /* for managing all SourceNode objects and for setting up SourceNode objects */
    class SourceNode : public Node
    {
        /* hide higher level methods from being used by this child class */
        private: using Node::get;
      

      public:
        SourceNode(std::string nickname, PaDeviceIndex device_index);
        ~SourceNode();

        double          getSampleRate();
        PaDeviceIndex   getDeviceIndex();
        short int       getTotalChannels();
        PaTime          getSuggestedLowLatency();
        PaTime          getSuggestedHighLatency();

      private:
        /* list of every SourceNode object, lookup by string identifer */
        inline static   std::unordered_map<std::string, SourceNode*> source_node_list;

        /* attributes of each SourceNode object */
        double          sample_rate;
        PaDeviceIndex   device_index;
        short int       total_channels;
        PaTime          suggested_low_latency;   
        PaTime          suggested_high_latency;
    };


    /* for managing all SinkNode objects and for setting up SinkNode objects */
    class SinkNode : public Node
    {
        /* hide higher level methods from being used by this child class */
        private: using Node::get;

        
      public:
        SinkNode(std::string nickname, PaDeviceIndex device_index);
        ~SinkNode();

        double          getSampleRate();
        PaDeviceIndex   getDeviceIndex();
        short int       getTotalChannels();
        PaTime          getSuggestedLowLatency();
        PaTime          getSuggestedHighLatency();

      private:
        /* list of every SinkNode object, lookup by string identifer */
        inline static   std::unordered_map<std::string, SinkNode*> sink_node_list; 

        /* attributes of each SinkNode object */
        double          sample_rate;
        PaDeviceIndex   device_index;
        short int       total_channels;
        PaTime          suggested_low_latency;   
        PaTime          suggested_high_latency;
    };
}
