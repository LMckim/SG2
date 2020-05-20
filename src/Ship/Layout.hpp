#ifndef SG_OBJECT_LAYOUT
#define SG_OBJECT_LAYOUT

#include <vector>
#include <src/Primitive/Node.hpp>

#include <src/Node/Floor.hpp>
#include <src/Node/Door.hpp>
#include <src/Node/Wall.hpp>

namespace SG::Generator{
    class GenPoly;
}

namespace SG::Ship
{
    using std::vector;
    using SG::Primitive::Node;
    using SG::Node::VisibleNode;

    using SG::Node::Floor;
    using SG::Node::Door;
    using SG::Node::Wall;

    class BaseShip;

    class Layout
    {
        friend class SG::Generator::GenPoly;
        friend class SG::Ship::BaseShip;

        public:
        Layout( vector< vector< Node* >> _nodes ) : nodes{ _nodes } 
        {
            auto itrY = this->nodes.begin();
            while( itrY != this->nodes.end() )
            {
                auto itrX = (*itrY).begin();
                while( itrX != (*itrY).end() )
                {
                    if(VisibleNode* vN = dynamic_cast< VisibleNode* >( (*itrX) ) )
                    {
                        this->visible.push_back( vN );
                    }
                    itrX++;
                }
                itrY++;
            }
        }
        virtual ~Layout() {}
        Node* getCenterNode()
        {
            return this->nodes[ MAX_HEIGHT / 2 ][ MAX_WIDTH / 2];
        }
        Floor* getRandomFloorNode()
        {
            Floor* inCase = NULL;
            for(size_t x=0; x < this->nodes.size(); x++)
            {
                for(size_t y=0; y < this->nodes[x].size(); y++)
                {
                    if(Floor* floor = dynamic_cast<Floor*>( this->nodes[x][y] ))
                    {
                        if(std::rand() % 77 == 0 && floor->occupied == false) return floor;
                        else inCase = floor;
                    }
                }
            }
            return inCase;
        }
        void clearPathing()
        {
            for(size_t x=0; x < this->nodes.size(); x++)
            {
                for(size_t y=0; y < this->nodes[x].size(); y++)
                {
                    this->nodes[x][y]->pathed = false;
                }
            }
        }
        void shiftPosition(int xOffset, int yOffset)
        {
            if(this->nodes.size() == 0) throw "Nodes have not been generated\n";
            for(size_t x=0; x < this->nodes.size(); x++)
            {
                for(size_t y=0; y < this->nodes[x].size(); y++)
                {
                    this->nodes[x][y]->move( xOffset, yOffset );
                }
            }
        }
        Node* findClosestNode( sf::Vector2f pos)
        {
            //TODO: pretty sure we can actually nail this with a tiny bit of math
            for(size_t y = 0; y < this->nodes.size(); y++)
            {
                for(size_t x = 0; x < this->nodes[y].size(); x++)
                {
                    Node* curr = this->nodes[y][x];
                    if( 
                    // if were within our bounds
                    (curr->position.y >= pos.y && curr->position.y <= (pos.y + NODE_SIZE) )
                    && (curr->position.x >= pos.x && curr->position.x <= (pos.x + NODE_SIZE) ) )
                    {
                        return this->nodes[y-1][x-1];
                    }
                }
            }
            // rare occasion?
            return nullptr;
        }
        protected:
        const int NODE_SIZE = 16;
        const int MAX_WIDTH = 120;
        const int MAX_HEIGHT = 68;
        Node* centerNode;
        vector< vector< Node* >> nodes;
        vector< VisibleNode* > visible; 
        void draw( sf::RenderTarget& target )
        {
            for( auto &itr : this->visible )
            {
                itr->draw( target );
            }
        }
        private:
    };
}


#endif