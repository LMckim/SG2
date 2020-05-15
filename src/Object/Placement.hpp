#ifndef SG_OBJECT_PLACEMENT
#define SG_OBJECT_PLACEMENT

#include <src/Manager/Object.hpp>
#include <src/Manager/Event.hpp>

#include <src/Object/BaseObject.hpp>
#include <src/Ship/Layout.hpp>

namespace SG::Window{
    class BaseWindow;
    class SpawnerWindow;
}

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
        friend class SG::Manager::Event;
        friend class SG::Window::BaseWindow;
        friend class SG::Window::SpawnerWindow;

        public:
        Placement( Object* _objectM, Layout* _layout, BaseObject* _spawnObject) 
            : objectM{ _objectM }, layout{ _layout }, spawnObject{ _spawnObject }
        {
            this->zLevel = Z_LAYERS::PLACEMENT;
            this->sprite.setTexture( *spawnObject->sprite.getTexture() );
            this->sprite.setOrigin(
                this->sprite.getLocalBounds().width / 2,
                this->sprite.getLocalBounds().height / 2
            );
            this->rotate();
            this->createPlacementOverlays();
        }
        virtual ~Placement()
        {
            std::cout << "ded\n";
        }
        void rotate()
        {
            // only allow 90 degree increments
            this->sprite.rotate(90);

        }
        virtual void forceDrag( sf::Vector2f mPos )
        {
            Node* closest = this->layout->findClosestNode( mPos);
            this->sprite.setPosition( closest->getPosition()->x, closest->getPosition()->y );
            this->badPlacement.setPosition( 
                sf::Vector2f(
                    this->sprite.getGlobalBounds().left,
                    this->sprite.getGlobalBounds().top
                ) 
            );
            this->goodPlacement.setPosition( 
                sf::Vector2f(
                    this->sprite.getGlobalBounds().left,
                    this->sprite.getGlobalBounds().top
                ) 
             );
        }
        virtual void stopDrag()
        {
            this->objectM->clearMe( dynamic_cast< Visible* >( this ) );
        }
        protected:
        const sf::Color GOOD_OVERLAY = sf::Color(0,255,0,100);
        const sf::Color BAD_OVERLAY = sf::Color(255,0,0,100);
        bool canPlace = false;

        Object* objectM;
        Layout* layout;
        BaseObject* spawnObject;

        sf::RectangleShape goodPlacement;
        sf::RectangleShape badPlacement;
        virtual void handleInput(sf::Event event) 
        {
            this->rotate();
        };

        void checkPlacement(Node* closest)
        {

        }
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