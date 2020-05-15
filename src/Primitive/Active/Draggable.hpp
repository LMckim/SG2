#ifndef SG_ACTIVE_DRAGGABLE
#define SG_ACTIVE_DRAGGABLE

#include <SFML/Graphics.hpp>
#include <src/Primitive/Active.hpp>
namespace SG::Manager{
    class Object;
    class Event;
}
namespace SG::Window{
    class BaseWindow;
    class SpawnerWindow;
}

namespace SG::Primitive
{
    using SG::Primitive::Active;
    class Draggable :
        virtual public Active
    {
        friend class SG::Manager::Object;
        friend class SG::Manager::Event;
        friend class SG::Window::SpawnerWindow;
        friend class SG::Window::BaseWindow;
        public:
        virtual ~Draggable() {}
        
        protected:
        virtual bool drag(sf::Vector2f mPos) { return true; };
        virtual void forceDrag(sf::Vector2f mPos) {};
        virtual void stopDrag() {};
    };
}


#endif