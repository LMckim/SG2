#ifndef SG_PRIMITIVE_GENERATOR
#define SG_PRIMITIVE_GENERATOR

#include <SFML/Graphics.hpp>

#include <src/Manager/Object.hpp>

using SG::Manager::Object;

namespace SG::Primitive
{
    class Generator
    {
        public:
        Generator(Object* _objectM) : objectM{ _objectM } {}
        virtual ~Generator() {}
        protected:
        Object* objectM;
        
    };
}


#endif