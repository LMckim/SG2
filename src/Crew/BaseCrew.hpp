#ifndef SG_CREW_BASE
#define SG_CREW_BASE

#include <vector>
#include <string>

#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Active.hpp>
#include <src/Primitive/Node.hpp>

#include <src/Node/Floor.hpp>

#include <src/Algo/AStar.hpp>

namespace SG::Manager{
    class Object;
}
namespace SG::Ship{
    class BaseShip;
}

namespace SG::Crew
{
    using std::vector;
    using std::string;
    using SG::Primitive::Node;
    using SG::Primitive::Variable;
    using SG::Primitive::Z_LAYERS;
    using SG::Primitive::Visible;
    using SG::Primitive::Active;

    using SG::Node::Floor;

    using SG::Algo::AStar;

    class Base :
        virtual public Visible,
        virtual public Variable,
        virtual public Active
    {
        friend class SG::Manager::Object;
        friend class SG::Ship::BaseShip;

        public:
        string name = "Dave";
        Base(sf::Texture* _spriteSheet, Node* _spawn) : currentNode{ _spawn }
        {
            this->zLevel = Z_LAYERS::CREW;

            this->groupselect = true;
            this->sprite.setTexture(*_spriteSheet);
            this->sprite.setPosition(this->currentNode->getPosition()->x, this->currentNode->getPosition()->y );
            this->currentNode->occupied = true;

            this->createBox(_spriteSheet);
        }
        virtual ~Base() {}

        virtual void setDestination( Node* _destination ) { this->destination = _destination; }
        virtual void update() 
        {
            // TODO: some cleanup here to deal with 2 guys vying for the same node, one will stop dead and no longer update
            // we could fix this by having them wait if their only node forward is blocked
            // however well need to have one take precedence or else theyll lock into a standoff
            if(this->path.size() > 0 && this->currDelay == 0)
            {
                this->currDelay = 2;
                // handle transition
                if(!this->path.back()->allowMove || this->path.back()->occupied)
                {
                    this->path = AStar::findPath( this->currentNode, this->destination );
                }else{
                    this->currentNode->occupied = false;
                    this->currentNode = this->path.back();
                    this->sprite.setPosition( this->currentNode->getPosition()->x, this->currentNode->getPosition()->y );
                    this->box.setPosition( this->currentNode->getPosition()->x, this->currentNode->getPosition()->y );
                    this->currentNode->occupied = true;

                    // cleanup
                    this->path.pop_back();
                }
            }else if( currDelay > 0 ) currDelay--;
        }

        protected:
        int currDelay = 0;
        bool showbox = true;
        const sf::Color OUTLINE_PLAIN = sf::Color::Red;
        const sf::Color OUTLINE_SELECTED = sf::Color::Magenta;
        const float OUTLINE_WIDTH = 0.75f;

        bool alive = true;
        bool wearingSpaceSuit = false;
        float oxygen = 100;
        float health = 100;
        float hunger = 0;
        float mass = 1;
        float morale = 100;
        float radiation = 0;

        enum FACING { LEFT, RIGHT, TOP, DOWN };
        FACING facing = DOWN;
        const float SPEED_WALK = 5.f;

        // pathing
        vector< Node* > path = vector< Node* >();
        Node* currentNode;
        Node* destination = nullptr;

        sf::RectangleShape box;

        using Active::select;
        virtual void select()
        {
            this->selected = true;
        }
        using Active::rightClick;
        virtual void rightClick(Node* dest)
        {
            // TODO: do a check to find other crew's destinations and adjust accordingly
            this->destination = dest;
            if(this->currentNode == dest)
            {
            }else this->path = AStar::findPath(this->currentNode, dest);
        }

        virtual void handleInput(sf::Event event) {}

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
            SG::Primitive::Visible::draw(target);
            if(this->selected) this->box.setOutlineColor( OUTLINE_SELECTED );
            else this->box.setOutlineColor( OUTLINE_PLAIN );
            if(this->showbox) target.draw( this->box );
        }
    };
}
#endif