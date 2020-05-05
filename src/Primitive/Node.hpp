#ifndef SG_PRIMITIVE_NODE
#define SG_PRIMITIVE_NODE

#include <SFML/Graphics.hpp>


namespace SG::Primitive
{
    class Node
    {
        struct Position{
            int x;
            int y;
        };

        public:
        ~Node() {};
        Node* getRightNode(){ return this->right; }
        Node* getLeftNode(){ return this->left; }
        Node* getTopNode(){ return this->top; }
        Node* getBottomNode(){ return this->bottom; }
        Position* getPosition() { return &this->position; }

        protected:
        bool allowMove = false;
        bool allowPlace = false;

        Position position;

        Node* right = NULL;
        Node* left = NULL;
        Node* top = NULL;
        Node* bottom = NULL;

        void setPosition( int x, int y ){ this->position.x = x; this->position.y = y; }
        void setRightNode(Node* node) { this->right = node; }
        void setLeftNode(Node* node) { this->left = node; }
        void setTopNode(Node* node) { this->top = node; }
        void setBottomNode(Node* node) { this->bottom = node; }
    };
}


#endif