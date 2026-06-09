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
    node_list.erase(this->nickname);
}

void node::Node::remove(std::string nickname)
{
    auto node = node_list.find(nickname);
    if (node != node_list.end()) delete node->second;
}

std::string node::Node::getNickname()
{
    return this->nickname;
}