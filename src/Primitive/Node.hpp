#ifndef SG_PRIMITIVE_NODE
#define SG_PRIMITIVE_NODE

#include <SFML/Graphics.hpp>

namespace SG::Generator{
    class Layout;
}
namespace SG::Primitive
{
    class Node
    {
        friend class SG::Generator::Layout;
        struct Position{
            int x;
            int y;
        };

        public:
        virtual ~Node() {};
        Node* getRightNode() { return this->right; }
        Node* getLeftNode() { return this->left; }
        Node* getTopNode() { return this->top; }
        Node* getBottomNode() { return this->bottom; }
        Position* getPosition() { return &this->position; }

        protected:
        enum LINKS { LEFT, RIGHT, TOP, BOTTOM };
        bool allowMove = false;
        bool allowPlace = false;
        bool spaceSuitReq = false;
        bool occupied = false;

        Position position;

        Node* right = NULL;
        Node* left = NULL;
        Node* top = NULL;
        Node* bottom = NULL;

        void setPosition( int x, int y ) { this->position.x = x; this->position.y = y; }
        void setRightNode(Node* node) { this->right = node; }
        void setLeftNode(Node* node) { this->left = node; }
        void setTopNode(Node* node) { this->top = node; }
        void setBottomNode(Node* node) { this->bottom = node; }

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
            }
        }
    };
}
#endif