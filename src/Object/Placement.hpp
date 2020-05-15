#ifndef SG_OBJECT_PLACEMENT
#define SG_OBJECT_PLACEMENT

#include <src/Manager/Object.hpp>

#include <src/Object/BaseObject.hpp>
#include <src/Ship/Layout.hpp>

namespace SG::Object
{
    using SG::Manager::Object;
    using SG::Primitive::Draggable;
    using SG::Primitive::Visible;
    using SG::Primitive::Z_LAYERS;
    using SG::Object::BaseObject;
    using SG::Primitive::Node;
    using SG::Ship::Layout;

    class Placement :
        virtual public Draggable,
        virtual public Visible
    {
        friend class SG::Manager::Object;

        public:
        Placement( Object* objectM, sf::Texture* texture, Layout* layout ) : objectM{ objectM }, layout{ layout }
        {
            this->zLevel = Z_LAYERS::PLACEMENT;
            this->sprite.setTexture( *texture );
            this->createPlacementOverlays();
        }
        virtual void forceDrag( sf::Vector2f mPos )
        {
            Node* closest = this->layout->findClosestNode( mPos);
            this->sprite.setPosition( closest->getPosition()->x, closest->getPosition()->y );
        }
        protected:
        const sf::Color GOOD_OVERLAY = sf::Color(0,255,0,100);
        const sf::Color BAD_OVERLAY = sf::Color(255,0,0,100);
        bool canPlace = false;

        Object* objectM;
        Layout* layout;

        sf::RectangleShape goodPlacement;
        sf::RectangleShape badPlacement;

        void createPlacementOverlays()
        {
            sf::Vector2f spriteSize = sf::Vector2f(
                this->sprite.getTexture()->getSize().x,         
                this->sprite.getTexture()->getSize().y       
            );
            this->goodPlacement.setSize(spriteSize);
            this->goodPlacement.setFillColor( GOOD_OVERLAY );
            this->goodPlacement.setPosition( this->sprite.getPosition() );

            this->badPlacement.setSize(spriteSize);
            this->badPlacement.setFillColor( BAD_OVERLAY );
            this->badPlacement.setPosition( this->sprite.getPosition() );
        }
        virtual void draw( sf::RenderTarget& target )
        {
            target.draw( this->sprite );
            if(this->canPlace) target.draw( this->goodPlacement );
            else target.draw( this->badPlacement );
        }
    };
}


#endif