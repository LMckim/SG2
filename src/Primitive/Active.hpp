#ifndef SG_PRIMITIVE_ACTIVE
#define SG_PRIMITIVE_ACTIVE

#include <SFML/Graphics.hpp>
#include <src/Primitive/Visible.hpp>
namespace SG::Manager{
    class Object;
}

namespace SG::Primitive
{
    using SG::Primitive::Visible;
    class Active :
        virtual public Visible
    {
        friend class SG::Manager::Object;
        public:
        virtual ~Active() {}
        
        protected:
        bool selected = false;
        bool groupselect = false;
        virtual void select() {};
        virtual void handleInput() = 0;
    };
}


#endif