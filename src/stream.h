#pragma once

#include <portaudio.h>
#include "nodes/node.h"

namespace node
{
    struct PairHash
    {
      size_t operator()(const std::pair<SourceNode*, SinkNode*>& p) const
      {
          size_t h1 = std::hash<SourceNode*>{}(p.first);
          size_t h2 = std::hash<SinkNode*>{}(p.second);
          return h1 ^ (h2 << 1);
      }
    };  

    class Stream
    {
      public:
        /* SourceNode to SinkNode constructor */
        Stream(node::SourceNode* source, node::SinkNode* sink);
        ~Stream();

        static void start();
        static void stop();
        static void remove(SourceNode* source, SinkNode* sink);

      private:
        inline static std::unordered_map<std::pair<SourceNode*, SinkNode*>, Stream*, PairHash> stream_list;

        PaStream*     stream_ptr;
        SourceNode*   input;
        SinkNode*     output;
    };
}
