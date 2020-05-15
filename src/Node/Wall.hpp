#ifndef SG_NODE_WALL
#define SG_NODE_WALL

#include <src/Node/Base/VisibleNode.hpp>

namespace SG::Node
{
    using SG::Node::Base::VisibleNode;

    class Wall :  
        virtual public VisibleNode
    {
        public:
        Wall(sf::Texture* _texture) : VisibleNode( _texture ) {}
        virtual ~Wall() {}
        protected:
    };
}
#endif