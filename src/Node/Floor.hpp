#ifndef SG_NODE_FLOOR
#define SG_NODE_FLOOR

#include <src/Node/Base/VisibleNode.hpp>

namespace SG::Node
{
    using SG::Node::Base::VisibleNode;

    class Floor :  
        virtual public VisibleNode
    {
        public:
        Floor(sf::Texture* texture) : VisibleNode( texture )
        {
            this->allowMove = true;
            this->allowPlace = true;
        }
        virtual ~Floor() {}
        protected:
    };
}
#endif