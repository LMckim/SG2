#ifndef SG_PRIMITIVE_ACTIVE
#define SG_PRIMITIVE_ACTIVE

#include <SFML/Graphics.hpp>
#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Node.hpp>

namespace SG::Manager{
    class Object;
}

namespace SG::Primitive
{
    using SG::Primitive::Visible;
    using SG::Primitive::Node;
    
    class Active :
        virtual public Visible
    {
        friend class SG::Manager::Object;
        public:
        virtual ~Active() {}
        
        protected:
        bool selected = false;
        bool groupselect = false;
        virtual void select( sf::Vector2f mpos ) {};
        // for non-context sensitive right-click actions
        virtual void leftClick() {};
        virtual void rightClick() {};
        // for destination or context sensitive right-click actions
        virtual void rightClick(Node* node) {};
        virtual void handleInput() = 0;
    };
}


#endif