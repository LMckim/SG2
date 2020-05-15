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
        Door(sf::Texture* _doorTexture, sf::Texture* _floorTexture) : VisibleNode( _doorTexture )
        {
            this->allowMove = true;
            this->floor.setTexture( *_floorTexture );
        }
        virtual ~Door() {}
        void toggleLocked()
        {
            this->allowMove == true ? this->allowMove = false : this->allowMove = true;
        }
        virtual void setPosition(int x, int y)
        {
            SG::Node::VisibleNode::setPosition(x,y);
            this->floor.setPosition(x,y);
        }
        protected:
        sf::Sprite floor = sf::Sprite();
        enum FACING { VERTICAL, HORIZONTAL };
        enum DIRERECTION_OF_MOVEMENT { UP, DOWN, LEFT, RIGHT };
        bool open = false;
        bool opening = false;
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->floor );
            SG::Node::VisibleNode::draw( target );
        }
        
    };
}
#endif