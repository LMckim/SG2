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
    using SG::Primitive::TILE_SIZE;
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
        Placement( Object* _objectM, Layout* _layout, BaseObject* _spawnObject ) 
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
             if(this->checkPlacement(closest)) this->canPlace = true;
             else this->canPlace = false;
        }
        virtual void stopDrag()
        {
            if(this->canPlace)
            {
                BaseObject* placedObj = new BaseObject( 
                    this->spawnObject,
                    this->layout->findClosestNode( this->sprite.getPosition() ),
                    this->sprite.getRotation()
                );
                placedObj->sprite.setPosition( this->sprite.getPosition() );
                // TODO: might need to change this to a variable
                this->objectM->addVisible( dynamic_cast< Visible* >( placedObj ));
            }
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

        bool checkPlacement(Node* closest)
        {
            uint16_t nodesX = this->spawnObject->sprite.getLocalBounds().width / TILE_SIZE;
            // uint16_t nodesY = this->spawnObject->sprite.getLocalBounds().height / TILE_SIZE;
            Node* checkY = closest;
            for(size_t y = 0; y < nodesX; y++)
            {
                Node* checkX = checkY;
                for(size_t x = 0; x < nodesX; x++)
                {
                    // check right side nodes are convetable to floor
                    if(!dynamic_cast< SG::Node::Floor* >(checkX) && checkX->isOccupied() == false)
                    {
                        return false;
                    }else checkX = checkX->getLeftNode();
                }
                // move to the next row
                checkY = checkY->getTopNode();
            }
            return true;
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