#include "node.h"

namespace node
{
    node::Node::Node(std::string nickname) // <-- thank you bjarne stroustrup for this beautiful syntax
    {
        this->nickname = nickname;
        node_list.insert({nickname, this});
    }
}

node::Node::~Node()
{
    node_list.erase(nickname);
}

std::string node::Node::getNickname()
{
    return this->nickname;
}