#ifndef SG_PRIMITIVE_VARIABLE
#define SG_PRIMITIVE_VARIABLE

#include <SFML/Graphics.hpp>

// forward declaration
namespace SG::Manager{
    class Object;
}

namespace SG::Primitive
{
    class Variable
    {
        friend class SG::Manager::Object;
        public:
        virtual ~Variable() {}

        protected:
        bool destroy = false;
        virtual void update() = 0;
    };
}


#endif