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
    using SG::Primitive::Variable;
    using SG::Primitive::Z_LAYERS;
    using SG::Tool::TextureSheet;
    using SG::Primitive::Node;

    class Placement;

    class BaseObject :
        virtual public Visible
    {
        friend class Placement;
        public:
        BaseObject( BaseObject* _base, Node* _parentNode, float _rotation = 0) : sheet{ _base->sheet }, parentNode{ _parentNode }
        {
            this->zLevel = Z_LAYERS::OBJECT;
            this->sprite.setTexture( *sheet->getTexture(0,0) );
            this->sprite.setOrigin(
                this->sprite.getLocalBounds().width / 2,
                this->sprite.getLocalBounds().height / 2
            );
            this->parentNode->setOccupied();
            this->sprite.setRotation( _rotation );

        }
        BaseObject(TextureSheet* _sheet) : sheet{ _sheet }
        {
            this->zLevel = Z_LAYERS::OBJECT;
            this->sprite.setTexture( *sheet->getTexture(0,0) );
        }
        ~BaseObject()
        {
            std::cout << "del_b_obj\n";
        }

        protected:
        TextureSheet* sheet;
        Node* parentNode;
    };

}


#endif