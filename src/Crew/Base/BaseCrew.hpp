#ifndef SG_CREW_BASE
#define SG_CREW_BASE

#include <vector>
#include <string>

#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Active.hpp>
#include <src/Primitive/Node.hpp>

#include <src/Node/Floor.hpp>

namespace SG::Manager{
    class Object;
}

namespace SG::Crew
{
    using std::vector;
    using std::string;
    using SG::Primitive::Node;
    using SG::Primitive::Variable;
    using SG::Primitive::Visible;
    using SG::Primitive::Active;

    using SG::Node::Floor;

    class Base :
        virtual public Visible,
        virtual public Variable,
        virtual public Active
    {
        friend class SG::Manager::Object;

        public:
        string name = "Dave";
        Base(sf::Texture* spriteSheet, Floor* spawn)
        {
            std::cout << "i live!\n";
            this->zLevel = 10;
            this->currentNode = spawn;

            this->groupselect = true;
            this->sprite.setTexture(*spriteSheet);
            this->sprite.setPosition(spawn->getPosition()->x, spawn->getPosition()->y );
            spawn->occupied = true;

            this->createBox(spriteSheet);
        }
        virtual ~Base() {}

        virtual void setDestination( Node* destination ) { this->destination = destination; }
        virtual void update() {}

        protected:
        bool showbox = true;
        const sf::Color OUTLINE_PLAIN = sf::Color::Red;
        const sf::Color OUTLINE_SELECTED = sf::Color::Magenta;
        const float OUTLINE_WIDTH = 0.75f;

        bool alive = true;
        float oxygen = 100;
        float health = 100;
        float hunger = 0;
        float mass = 1;
        float morale = 100;
        float radiation = 0;

        enum FACING { LEFT, RIGHT, TOP, DOWN };
        const float SPEED_WALK = 5.f;
        Node* currentNode;
        Node* intermediate = nullptr;
        Node* destination = nullptr;

        sf::RectangleShape box;

        virtual void select()
        {
            this->selected = true;
            std::cout << "My name is: " << this->name << std::endl;
        }

        virtual void rightClick(Node* dest)
        {
            this->destination = dest;
        }

        virtual void handleInput() {}
        vector< Node* > findPath(vector< Node* > path)
        {
            Node* curr = this->currentNode;
            Node* dest = this->destination;
            if( curr->getPosition()->x < dest->getPosition()->x )
            {
                if( curr->getLeftNode()->canMoveTo() )
                {
                    path.push_back( curr->getLeftNode() );
                }
            }else if( curr->getPosition()->x > dest->getPosition()->x ){

            }
            // recursively find path
            if( path[ path.size() ] == dest ) return path;
            else return this->findPath( path );
        }
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