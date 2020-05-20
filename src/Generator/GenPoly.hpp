#ifndef SG_GENERATOR_POLY
#define SG_GENERATOR_POLY

#include <src/Primitive/Generator.hpp>
#include <src/Ship/Layout.hpp>
#include <src/Primitive/Visible.hpp>

#include <src/Node/Wall.hpp>
#include <src/Node/Floor.hpp>
#include <src/Node/Door.hpp>
#include <src/Node/Space.hpp>

namespace SG::Generator
{
    using std::vector;
    using SG::Primitive::Generator;
    using SG::Ship::Layout;
    using SG::Primitive::Node;
    using SG::Node::Floor;
    using SG::Node::Wall;

    class GenPoly   :
        public Generator
    {
        public:
        vector< sf::VertexArray > buildPolygons( Layout* _layout )
        {
            // our ooutput
            vector< sf::VertexArray > polygons;
            // get the top left node
            Node* currentNode = _layout->nodes[0][0];

            // go through and built out all of our vertex points and edges
            while(currentNode != nullptr)
            {
                while(currentNode->getRightNode() != nullptr)
                {
                    if(Wall* wall = dynamic_cast< Wall* >( currentNode ))
                    {
                            
                    }
                }
            }
        }
        protected:
    };
}

#endif