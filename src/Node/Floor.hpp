#ifndef SG_NODE_FLOOR
#define SG_NODE_FLOOR

#include <src/Primitive/Node.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Node
{
    using SG::Primitive::Node;
    using SG::Primitive::Visible;

    class Floor :  
        virtual public Node,
        virtual public Visible
    {
        public:
        Floor(sf::Texture* texture)
        {
            this->allowMove = true;
            this->allowPlace = true;
            
            this->zLevel = 5;
            this->sprite.setTexture( *texture );
        }
        virtual void setPosition(int x, int y) override
        {
            this->sprite.setPosition(x,y);
        }
        virtual ~Floor() {}
        protected:
        

    };
}
#endif