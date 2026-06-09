#pragma once

#include <portaudio.h>
#include "node/node.h"

namespace node
{
    struct PairHash
    {
      size_t operator()(const std::pair<InputNode*, SinkNode*>& p) const
      {
          size_t h1 = std::hash<InputNode*>{}(p.first);
          size_t h2 = std::hash<SinkNode*>{}(p.second);
          return h1 ^ (h2 << 1);
      }
    };  

    class Stream
    {
      public:
        /* InputNode to SinkNode constructor */
        Stream(node::InputNode* source, node::SinkNode* sink);
        ~Stream();

        static void start();
        static void stop();
        static void remove(InputNode* source, SinkNode* sink);

      private:
        inline static std::unordered_map<std::pair<InputNode*, SinkNode*>, Stream*, PairHash> stream_list;

        PaStream*     stream_ptr;
        InputNode*   input;
        SinkNode*     output;
    };
}
