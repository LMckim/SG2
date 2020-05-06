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
        virtual ~Active() {}
        
        protected:
        bool selected = false;
        bool groupselect = false;
        virtual void onSelect() {};
        virtual void handleInput() = 0;
    };
}


#endif