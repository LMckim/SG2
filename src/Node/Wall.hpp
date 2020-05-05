#ifndef SG_NODE_WALL
#define SG_NODE_WALL

#include <src/Primitive/Node.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Node
{
    using SG::Primitive::Node;
    using SG::Primitive::Visible;

    class Wall :  
        virtual protected Node
    {
        public:
        Wall(Visible* visible)
        {
            this->visible = visible;
        }
        ~Wall() {}
        protected:
        Visible* visible;
        

    };
}
#endif