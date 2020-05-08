#ifndef SG_NODE_SPACE
#define SG_NODE_SPACE

#include <src/Primitive/Node.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Node
{
    using SG::Primitive::Node;

    class Space :  
        virtual public Node
    {
        public:
        Space()
        {
            this->allowMove = false;
            this->spaceSuitReq = true;
        }
        virtual ~Space() {}
        protected:        

    };
}
#endif