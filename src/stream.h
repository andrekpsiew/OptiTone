#pragma once

#include <portaudio.h>
#include "nodes/node.h"


namespace node
{
    class Stream
    {
      public:
        /* SourceNode to SinkNode constructor */
        Stream(node::SourceNode* source, node::SinkNode* sink);

      private:

    };
}