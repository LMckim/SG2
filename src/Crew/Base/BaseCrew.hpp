#ifndef SG_CREW_BASE
#define SG_CREW_BASE

#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Node.hpp>

namespace SG::Crew
{
    using SG::Primitive::Node;
    using SG::Primitive::Variable;
    using SG::Primitive::Visible;

    class Base :
        virtual public Visible,
        virtual public Variable
    {
        public:
        
        Base(sf::Image* spriteSheet, Node* spawn)
        {
            this->zLevel = 10;
            this->currentNode = spawn;
        }
        virtual ~Base() {}

        virtual void setDestination( Node* destination ) { this->destination = destination; }

        protected:
        enum FACING { LEFT, RIGHT, TOP, DOWN };
        const float SPEED_WALK = 5;
        Node* currentNode;
        Node* destination;
    };
}


#endif