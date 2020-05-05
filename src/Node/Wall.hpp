#ifndef SG_NODE_WALL
#define SG_NODE_WALL

#include <src/Primitive/Node.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Node
{
    using SG::Primitive::Node;
    using SG::Primitive::Visible;

    class Wall :  
        virtual public Node,
        virtual public Visible
    {
        public:
        Wall(sf::Texture* texture)
        {
            this->zLevel = 6;
            this->sprite.setTexture( *texture );
        }
        virtual void setPosition(int x, int y) override
        {
            this->sprite.setPosition(x,y);
        }
        virtual ~Wall() {}
        protected:
        

    };
}
#endif