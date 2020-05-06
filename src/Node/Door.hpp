#ifndef SG_NODE_DOOR
#define SG_NODE_DOOR

#include <src/Node/Base/VisibleNode.hpp>

namespace SG::Node
{
    using SG::Node::Base::VisibleNode;

    class Door :  
        virtual public VisibleNode
    {
        public:
        Door(sf::Texture* texture) : VisibleNode( texture )
        {
            this->allowMove = true;
        }
        virtual ~Door() {}
        protected:
        enum DIRECTION { VERTICAL, HORIZONTAL };
        bool locked = false;
        bool open = false;
        bool opening = false;
    };
}
#endif