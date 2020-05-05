#ifndef SG_NODE_FLOOR
#define SG_NODE_FLOOR

#include <src/Primitive/Node.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Node
{
    using SG::Primitive::Node;
    using SG::Primitive::Visible;

    class Floor :  
        virtual protected Node
    {
        public:
        Floor(Visible* visible)
        {
            this->allowMove = true;
            this->allowPlace = true;
            this->visible = visible;
        }
        ~Floor() {}
        protected:
        Visible* visible;
        

    };
}
#endif