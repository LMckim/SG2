#ifndef SG_ALGO_ASTAR
#define SG_ALGO_ASTAR

#include <vector>
#include <src/Primitive/Node.hpp>

namespace SG::Algo
{
    using std::vector;
    using SG::Primitive::Node;

    class PathNode
    {
        public:
        float h;
        Node* current = nullptr;
        PathNode* parent = nullptr;
        Node* destination = nullptr;
        PathNode() {};
        PathNode(Node* current, PathNode* parent, Node* destination) : current{ current }, parent{ parent }
        {
            this->h = ( current->getPosition()->x - destination->getPosition()->x )
                    + ( current->getPosition()->y - destination->getPosition()->y );
            if(this->h < 0 ) this->h = this-> h * -1;
            this->destination = destination;
        }           
        vector< PathNode > getAdjacent()
        {
            vector< PathNode > adjacent;
            if(this->current->left->allowMove && !this->current->left->occupied){
                adjacent.push_back( PathNode( this->current->left, this, this->destination ) );
            }
            if(this->current->right->allowMove && !this->current->right->occupied){
                adjacent.push_back( PathNode( this->current->right, this, this->destination ) );
            }
            if(this->current->top->allowMove && !this->current->top->occupied){
                adjacent.push_back( PathNode( this->current->top, this, this->destination ) );
            }
            if(this->current->bottom->allowMove && !this->current->bottom->occupied){
                adjacent.push_back( PathNode( this->current->bottom, this, this->destination ) );
            }
            return adjacent;
        }
        bool operator==(const PathNode& r)
        {
            if(r.current == this->current){
                return true;
            }else return false;
        }
    };

    class AStar
    {
        public:

        static vector< Node* > findPath(Node* currentNode, Node* destinationNode)
        {
            // the path well pass back, if no path can be found then return empty
            vector< Node* > path;
            // nodes which are still open to be searched
            vector< PathNode > open;
            open.push_back( PathNode(currentNode, nullptr, destinationNode) );

            // nodes that have been searched
            vector< PathNode > closed;
            while( !open.empty() )
            {
                // get the current closest node to be searched
                PathNode current = AStar::lowestHeuristic( open );
                // make sure we dont search this node again
                closed.push_back( current );
                // remove the node from the open list as its about to be searched
                // and we dont want to searh it again
                AStar::removeFromVector( open, current );
                // check if this node contains our goal
                if(current.current == destinationNode)
                {
                    AStar::buildPath( path, current );
                    break;
                }

                // get all the adjacent nodes
                vector< PathNode > adjacent = current.getAdjacent();
                for( auto &itr : adjacent )
                {
                    itr.current->pathed = true;
                    // if weve already searched this node then ignore it
                    if( AStar::inVector( closed, itr ) ) continue;
                    // if its already in the open vecotr.... do. something?
                    else if( AStar::inVector( open, itr ) ){
                    
                    // add the new-found node to the open list
                    }else open.push_back( itr );
                }

            }
            // return the found path or empty if none found
            return path;
        }
        private:

        static bool inVector( vector< PathNode > vec, PathNode pN )
        {
            for(auto &itr : vec)
            {
                if(itr == pN) return true;
            }
            return false;
        }
        static void removeFromVector( vector< PathNode >& vec, PathNode pN )
        {
            for(size_t i=0; i < vec.size(); i++)
            {
                vec.erase( vec.begin() + i );
                break;
            }
        }
        // retrieve the node with the current lowest distance to target
        static PathNode lowestHeuristic(vector< PathNode > list)
        {
            PathNode lowest;
            for( auto &itr : list )
            {
                if( lowest.current == NULL )
                {
                    lowest = itr;  
                }else if( lowest.h > itr.h ){
                    lowest = itr;
                }
            }
            return lowest;
        }

        static void buildPath(vector< Node* >& path, PathNode pN)
        {
            while(pN.parent != nullptr)
            {
                path.push_back( pN.current );
                pN.current->pathed = true;
                pN = *pN.parent;
            }
        }
    };
}

#endif