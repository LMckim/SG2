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
            this->createBox( texture );
        }
        virtual void setPosition(int x, int y) override
        {
            this->position.x = x;
            this->position.y = y;
            this->sprite.setPosition(x,y);
            this->box.setPosition(x,y);
        }
        virtual void move(int x, int y)
        {
            this->position.x += x;
            this->position.y += y;
            this->sprite.setPosition( this->position.x, this->position.y );
            this->box.setPosition( this->position.x, this->position.y );
        }
        virtual ~VisibleNode() {}
        protected:
        bool showBox = false;
        const sf::Color OUTLINE_PLAIN = sf::Color::Cyan;
        const sf::Color OUTLINE_OCCUPIED = sf::Color::Magenta;
        const sf::Color OUTLINE_PATHED = sf::Color::Green;
        const float OUTLINE_WIDTH = 0.75f;
        sf::RectangleShape box;

        void createBox(sf::Texture* tex)
        {
            this->box = sf::RectangleShape();
            this->box.setSize(
                sf::Vector2f(
                    tex->getSize().x,
                    tex->getSize().y
                )
            );
            this->box.setFillColor( sf::Color::Transparent );
            this->box.setOutlineColor( OUTLINE_PLAIN );
            this->box.setOutlineThickness( OUTLINE_WIDTH );
            this->box.setPosition( this->sprite.getPosition() );
        }
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->sprite );
            // draw the box ON TOP OF the sprite
            if(this->showBox){
                if(this->occupied) {
                    this->box.setOutlineColor( OUTLINE_OCCUPIED );
                }else if(this->pathed){ 
                    this->box.setOutlineColor( OUTLINE_PATHED );
                }else this->box.setOutlineColor( OUTLINE_PLAIN );
                target.draw( this->box );
            }
        }
    };
}
#endif