#ifndef SG_PRIMITIVE_NODE
#define SG_PRIMITIVE_NODE

#include <SFML/Graphics.hpp>

namespace SG::Generator{
    class Gen_Layout;
}
namespace SG::Crew{
    class Base;
}
namespace SG::Ship{
    class Layout;
}
namespace SG::Algo{
    class AStar;
    class PathNode;
}
namespace SG::Primitive
{
    enum NODE_TYPE { FLOOR, WALL, DOOR, SPACE };
    class Node
    {
        friend class SG::Generator::Gen_Layout;
        friend class SG::Crew::Base;
        friend class SG::Ship::Layout;
        friend class SG::Algo::AStar;
        friend class SG::Algo::PathNode;
        
        struct Position{
            int x;
            int y;
        };

        public:
        virtual ~Node() {};
        bool canMoveTo()
        {
            if(this->allowMove && this->occupied == false) return true;
            else return false;
        }
        Node* getRightNode() { return this->right; }
        Node* getLeftNode() { return this->left; }
        Node* getTopNode() { return this->top; }
        Node* getBottomNode() { return this->bottom; }
        Position* getPosition() { return &this->position; }
        bool isOccupied() { return this->occupied; }


        protected:
        enum LINKS { LEFT, RIGHT, TOP, BOTTOM };
        bool pathed = false;
        bool allowMove = false;
        bool allowPlace = false;
        bool spaceSuitReq = false;
        bool occupied = false;

        Position position;

        Node* right = nullptr;
        Node* left = nullptr;
        Node* top = nullptr;
        Node* bottom = nullptr;

        virtual void setPosition( int x, int y ) { this->position.x = x; this->position.y = y; }
        void setRightNode(Node* node) { this->right = node; }
        void setLeftNode(Node* node) { this->left = node; }
        void setTopNode(Node* node) { this->top = node; }
        void setBottomNode(Node* node) { this->bottom = node; }

        virtual void move(int x, int y)
        {
            this->position.x += x;
            this->position.y += y;
        }

        void link(LINKS direction, Node* linkNode)
        {
            switch(direction)
            {
                case LEFT:
                linkNode->right = this;
                this->left = linkNode;
                break;

                case RIGHT:
                linkNode->left = this;
                this->right = linkNode;
                break;

                case TOP:
                linkNode->bottom = this;
                this->top = linkNode;
                break;

                case BOTTOM:
                linkNode->top = this;
                this->bottom = linkNode;
                break;

                default:
                throw "No direction set";
            }
        }
    };
}
#endif