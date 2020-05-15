#ifndef SG_OBJECT_BASE
#define SG_OBJECT_BASE

#include <src/Primitive/Active/Draggable.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Node.hpp>
namespace SG::Object
{
    using SG::Primitive::Draggable;
    using SG::Primitive::Visible;
    using SG::Primitive::Z_LAYERS;

    class BaseObject :
        virtual public Visible
    {
        public:
        BaseObject( )
        {
            this->zLevel = Z_LAYERS::OBJECT;
        }
        virtual ~BaseObject() {}

        protected:
    };

}


#endif