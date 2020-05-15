#ifndef SG_OBJECT_BASE
#define SG_OBJECT_BASE

#include <src/Tool/TextureSheet.hpp>
#include <src/Primitive/Active/Draggable.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Node.hpp>
namespace SG::Object
{
    using SG::Primitive::Draggable;
    using SG::Primitive::Visible;
    using SG::Primitive::Z_LAYERS;
    using SG::Tool::TextureSheet;

    class Placement;

    class BaseObject :
        virtual public Visible
    {
        friend class Placement;
        public:
        BaseObject(TextureSheet* sheet) : sheet{ sheet }
        {
            this->zLevel = Z_LAYERS::OBJECT;
            this->sprite.setTexture( *sheet->getTexture(0,0) );
        }
        virtual ~BaseObject() {}

        protected:
        TextureSheet* sheet;
    };

}


#endif