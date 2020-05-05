#ifndef SG_NODE_DOOR
#define SG_NODE_DOOR

#include <src/Primitive/Node.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Node
{
    using SG::Primitive::Node;
    using SG::Primitive::Visible;

    class Door :  
        virtual public Node,
        virtual public Visible
    {
        public:
        Door(sf::Texture* texture)
        {
            this->zLevel = 6;
            this->sprite.setTexture( *texture );
        }
        virtual ~Door() {}
        protected:
    };
}
#endif