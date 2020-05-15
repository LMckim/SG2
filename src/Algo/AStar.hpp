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
        PathNode(const PathNode& orig)
        {
            this->h = orig.h;
            this->current = orig.current;
            this->parent = orig.parent;
            this->destination = orig.destination;
        }
        PathNode(Node* _current, PathNode* _parent, Node* _destination) : current{ _current }, parent{ _parent }, destination{ _destination }
        {
            int x = ( current->getPosition()->x - this->destination->getPosition()->x );
            int y = ( current->getPosition()->y - this->destination->getPosition()->y );

            // normalize the results to always be positive
            if( x < 0 ) x *= -1;
            if( y < 0 ) y *= -1;

            this->h = x + y;
        }
        ~PathNode()
        {
            
        }         
        vector< PathNode* > getAdjacent()
        {
            vector< PathNode* > adjacent;
            if(this->current->left != nullptr && this->current->left->allowMove && !this->current->left->occupied){
                adjacent.push_back( new PathNode( this->current->left, this, this->destination ) );
            }
            if(this->current->right != nullptr &&  this->current->right->allowMove && !this->current->right->occupied){
                adjacent.push_back( new PathNode( this->current->right, this, this->destination ) );
            }
            if(this->current->top != nullptr &&  this->current->top->allowMove && !this->current->top->occupied){
                adjacent.push_back( new PathNode( this->current->top, this, this->destination ) );
            }
            if(this->current->bottom != nullptr && this->current->bottom->allowMove && !this->current->bottom->occupied){
                adjacent.push_back( new PathNode( this->current->bottom, this, this->destination ) );
            }
            return adjacent;
        }
        float getTotalHeuristicCost()
        {
            float total = 0;
            PathNode* pN = this;
            while(pN->parent != nullptr)
            {
                total += pN->h;
                pN = pN->parent;
            }
            return total;
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
            vector< PathNode* > open;
            open.push_back( new PathNode(currentNode, nullptr, destinationNode) );

            // nodes that have been searched
            vector< PathNode* > closed;
            while( !open.empty() )
            {
                // get the current closest node to be searched
                PathNode* current = AStar::lowestHeuristic( open );
                // make sure we dont search this node again
                closed.push_back( current );
                // remove the node from the open list as its about to be searched
                // and we dont want to searh it again
                AStar::removeFromVector( open, current );
                // check if this node contains our goal
                if(current->current == destinationNode)
                {
                    AStar::buildPath( path, current );
                    break;
                }

                // get all the adjacent nodes
                vector< PathNode* > adjacent = current->getAdjacent();
                for( auto &itr : adjacent )
                {
                    // if weve already searched this node then ignore it
                    if( AStar::inVector( closed, itr ) ){
                        delete itr;
                        continue;
                    // if its already in the open vector
                    // check if its a better match for the path than the current itr
                    // if so then delete the stored node and add the better candidate to the open vector
                    }else if(PathNode* nP = AStar::inVector( open, itr ) ){
                        // if the existing open node is worse than the current one then replace it
                        if(nP->getTotalHeuristicCost() > itr->getTotalHeuristicCost())
                        {
                            AStar::removeFromVector( open, nP );
                            open.push_back( itr );
                            delete nP;
                        }else delete itr;
                    // add the new-found node to the open list
                    }else{ open.push_back( itr ); }
                }
                adjacent.clear();

            }
            // cleanup
            while( !open.empty() ) delete open.back(), open.pop_back();
            while( !closed.empty() ) delete closed.back(), closed.pop_back();
            // return the found path or empty if none found
            return path;
        }
        private:
        static PathNode* inVector(const vector< PathNode* > vec, const PathNode* pN )
        {
            for(auto &itr : vec)
            {
                if(itr->current == pN->current) return itr;
            }
            return NULL;
        }
        static void removeFromVector( vector< PathNode* >& vec, PathNode* pN )
        {
            for(size_t i=0; i < vec.size(); i++)
            {
                if( vec[i] == pN ){
                    vec.erase( vec.begin() + i );
                    break;
                }
            }
        }
        // retrieve the node with the current lowest distance to target
        static PathNode* lowestHeuristic(vector< PathNode* > list)
        {
            PathNode* lowest = list[0];
            for( auto &itr : list )
            {
                if( lowest->getTotalHeuristicCost() > itr->getTotalHeuristicCost() ){
                    lowest = itr;
                }
            }
            return lowest;
        }

        static void buildPath(vector< Node* >& path, PathNode *pN)
        {
            while(pN->parent != nullptr)
            {
                path.push_back( pN->current );
                pN->current->pathed = true;
                pN = pN->parent;
            }
        }
    };
}

#endif