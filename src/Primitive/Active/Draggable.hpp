#ifndef SG_ACTIVE_DRAGGABLE
#define SG_ACTIVE_DRAGGABLE

#include <SFML/Graphics.hpp>
#include <src/Primitive/Active.hpp>
namespace SG::Manager{
    class Object;
}

namespace SG::Primitive
{
    using SG::Primitive::Active;
    class Draggable :
        virtual public Active
    {
        friend class SG::Manager::Object;
        public:
        virtual ~Draggable() {}
        
        protected:
        virtual bool drag(sf::Vector2f mPos) { return false; };
        virtual void forceDrag(sf::Vector2f mPos) {};
    };
}


#endif