#ifndef SG_NODE_BASE_VISIBLE
#define SG_NODE_BASE_VISIBLE

#include <src/Primitive/Node.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Node::Base
{
    using SG::Primitive::Node;
    using SG::Primitive::Visible;

    class VisibleNode :  
        virtual public Node,
        virtual public Visible
    {
        public:
        VisibleNode(sf::Texture* texture)
        {           
            this->zLevel = 5;
            this->sprite.setTexture( *texture );
        }
        virtual void setPosition(int x, int y) override
        {
            this->position.x = x;
            this->position.y = y;
            this->sprite.setPosition(x,y);
        }
        virtual void move(int x, int y)
        {
            this->position.x += x;
            this->position.y += y;
            this->sprite.setPosition( this->position.x, this->position.y );
        }
        virtual ~VisibleNode() {}
        protected:
        

    };
}
#endif