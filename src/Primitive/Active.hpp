#ifndef SG_PRIMITIVE_ACTIVE
#define SG_PRIMITIVE_ACTIVE

#include <SFML/Graphics.hpp>

namespace SG::Manager{
    class Object;
}

namespace SG::Primitive
{
    class Active
    {
        friend class SG::Manager::Object;
        public:
        ~Active() {}
        
        protected:
        bool selected = false;
        virtual void handleInput() = 0;
    };
}


#endif