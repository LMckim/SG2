#ifndef SG_GENERATOR
#define SG_GENERATOR

#include <map>
#include <src/Primitive/Generator.hpp>
#include <src/Ship/Layout.hpp>
#include <src/Tool/TextureSheet.hpp>
#include <src/Primitive/Visible.hpp>

#include <src/Node/Wall.hpp>
#include <src/Node/Floor.hpp>
#include <src/Node/Door.hpp>
#include <src/Node/Space.hpp>

namespace SG::Generator
{
    using std::vector;
    using std::map;
    
    using SG::Manager::Object;
    using SG::Primitive::Node;
    using SG::Primitive::Generator;
    using SG::Primitive::Visible;

    using SG::Node::Floor;
    using SG::Node::Wall;
    using SG::Node::Door;
    using SG::Node::Space;

    using SG::Tool::TextureSheet;

    class Gen_Layout :
        protected Generator
    {
        public:
        const int TILE_SIZE = 16;
        enum TILE { SPACE, FLOOR, WALL, DOOR };
        
        Gen_Layout( Object* _objectM ) : Generator::Generator( _objectM ) {}
        ~Gen_Layout() {}

        void addAsset(TILE tile, TextureSheet* asset)
        {
            this->assets[tile].push_back(asset);
        }

        SG::Ship::Layout generateLayout(sf::Image* layout)
        {
            // we need graphical 
            if( this->assets.find( TILE::DOOR ) == this->assets.end() 
            ||  this->assets.find( TILE::FLOOR ) == this->assets.end()
            ||  this->assets.find( TILE::WALL ) == this->assets.end() )
            { throw "Not enough assets to generate layout"; }

            uint16_t yS = layout->getSize().y;
            uint16_t xS = layout->getSize().x;

            // each vertical row
            for( size_t y=0; y < yS; y++)
            {   
                // each node within the horizontal row
                this->nodes.push_back(vector< Node* >());
                for(size_t x=0; x < xS; x++)
                {
                    // add nodes to our grid
                    Node* newNode;
                    switch( this->checkColor( layout->getPixel(x,y) ))
                    {
                        case FLOOR:
                            newNode = generateFloorNode( layout, x, y );
                        break;
                        case WALL:
                            newNode = generateWallNode( layout, x, y );
                        break;
                        case DOOR:
                            newNode = generateDoorNode( layout, x, y );
                        break;
                        default:
                            newNode = new Space();
                    }
                    // if its a visible node then add it to the screen
                    if(Visible* visible = dynamic_cast< Visible* >(newNode))
                    {
                        this->objectM->addVisible( visible );
                    }
                    newNode->setPosition( x * TILE_SIZE, y * TILE_SIZE );
                    this->nodes[y].push_back( newNode );
                    if(x > 0) newNode->link( Node::LINKS::LEFT, this->nodes[y][x-1] );
                    if(y > 0) newNode->link( Node::LINKS::TOP, this->nodes[y-1][x] );
                }
            }
            return SG::Ship::Layout(this->nodes);
        }
        protected:
        map< TILE, vector< TextureSheet* >> assets;
        vector< vector< Node* >> nodes;


        private:
        TILE checkColor(const sf::Color &color)
        {
            // white
            if(color.r == 255 && color.g == 255 && color.b == 255 && color.a == 255){
                return FLOOR;
            // cyan
            }else if(color.r == 0 && color.g == 255 && color.b == 255 && color.a == 255){
                return WALL;
            // Transparent
            }else if(color.r == 0 && color.g == 0 && color.b == 0 && color.a == 0){
                return SPACE;
            // red
            }else if(color.r == 255 && color.g == 0 && color.b == 0 && color.a == 255){
                return DOOR;
            // default to space node
            }else{
                return SPACE;
            }
        }

        Node* generateWallNode(sf::Image* layout, int xPos, int yPos )
        {
            // figure out what section of wall we need and how we need to rotate it
            TILE top = this->checkColor( layout->getPixel( xPos, yPos - 1 ) );
            TILE left = this->checkColor( layout->getPixel( xPos - 1, yPos) );
            TILE right = this->checkColor( layout->getPixel( xPos + 1, yPos) );
            TILE bottom = this->checkColor( layout->getPixel( xPos, yPos + 1 ) );

            // god... this. is. ugly.
            Node* wall;
            // surrounded by other walls
            if(top == WALL 
            && bottom == WALL 
            && left == WALL 
            && right == WALL)
            {
                wall = new Wall( this->assets[WALL][0]->getTexture(0,3) );

            // isolated wall
            }else if(top == FLOOR 
                  && bottom == FLOOR 
                  && left == FLOOR 
                  && right == FLOOR){
                wall = new Wall( this->assets[WALL][0]->getTexture(0,4) );

            // straight up wall section
            }else if(top == WALL
                  && bottom == WALL
                  && left == FLOOR
                  && right == FLOOR){
                wall = new Wall( this->assets[WALL][0]->getTexture(0,0) );

            // straight across wall section
            }else if(top == FLOOR
                  && bottom == FLOOR 
                  && left == WALL 
                  && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(0,1) );

            // tri-section down
            }else if((top == FLOOR || top == DOOR) 
                   && bottom == WALL 
                   && left == WALL 
                   && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(1,0) );

            // tri-section left
            }else if(top == WALL 
                  && bottom == WALL 
                  && left == WALL 
                  && (right == FLOOR || right == DOOR) ){
                wall = new Wall( this->assets[WALL][0]->getTexture(1,1) );

            // tri-section top
            }else if(top == WALL 
                  && (bottom == FLOOR || bottom  == DOOR) 
                  && left == WALL 
                  && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(1,2) );

            // tri-section right
            }else if(top == WALL 
                  && bottom == WALL 
                  && (left == FLOOR || left == DOOR) 
                  && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(1,3) );

            // edge section right
            }else if((top == DOOR || top == FLOOR) 
                  && (bottom == DOOR || bottom == FLOOR) 
                  && left == WALL 
                  && (right == DOOR || right == FLOOR)){
                wall = new Wall( this->assets[WALL][0]->getTexture(2,0) );

            // edge section down
            }else if(top == WALL 
                 && (bottom == DOOR || bottom == FLOOR) 
                 && (left == DOOR || left == FLOOR) 
                 && (right == DOOR || right == FLOOR)){
                wall = new Wall( this->assets[WALL][0]->getTexture(2,1) );

            // edge section left
            }else if((top == DOOR || top == FLOOR) 
                  && (bottom == DOOR || bottom == FLOOR) 
                  && (left == DOOR || left == FLOOR) 
                  && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(2,2) );

            // edge section up
            }else if((top == DOOR || top == FLOOR) 
                  && bottom == WALL 
                  && (left == DOOR || left == FLOOR) 
                  && (right == DOOR || right == FLOOR)){
                wall = new Wall( this->assets[WALL][0]->getTexture(2,3) );

            // corner bl
            }else if(top == WALL 
                 && (bottom == DOOR || bottom == FLOOR) 
                 && (left == DOOR || left == FLOOR) 
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(3,0) );

            // corner tl
            }else if((top == DOOR || top == FLOOR) 
                   && bottom == WALL 
                   && (left == DOOR || left == FLOOR) 
                   && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(3,1) );

            // corner tr
            }else if((top == DOOR || top == FLOOR) 
                   && bottom == WALL 
                   && left == WALL 
                   && (right == DOOR || right == FLOOR)){
                wall = new Wall( this->assets[WALL][0]->getTexture(3,2) );

            // corner br
            }else if(top == WALL 
                 && (bottom == DOOR || bottom == FLOOR) 
                 && left == WALL 
                 && (right == DOOR || right == FLOOR)){
                wall = new Wall( this->assets[WALL][0]->getTexture(3,3) );

            // space vertical right wall
            }else if(top == WALL 
                 && bottom == WALL 
                 && left == SPACE 
                 && right == FLOOR){
                wall = new Wall( this->assets[WALL][0]->getTexture(4,0) );

            // space veritcal left wall
            }else if(top == WALL 
                 && bottom == WALL 
                 && left == FLOOR
                 && right == SPACE){
                wall = new Wall( this->assets[WALL][0]->getTexture(4,3) );

            // space horizontal top wall
            }else if(top == FLOOR 
                 && bottom == SPACE
                 && left == WALL
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(4,1) );

            // space horizontal bottom wall
            }else if(top == SPACE 
                 && bottom == FLOOR 
                 && left == WALL
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(4,2) );

            // space br corner
            }else if(top == SPACE 
                 && bottom == WALL 
                 && left == SPACE
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(5,3) );

            // space bl corner
            }else if(top == SPACE 
                 && bottom == WALL 
                 && left == WALL
                 && right == SPACE){
                wall = new Wall( this->assets[WALL][0]->getTexture(5,2) );

            // space tl corner
            }else if(top == WALL 
                 && bottom == SPACE 
                 && left == WALL
                 && right == SPACE){
                wall = new Wall( this->assets[WALL][0]->getTexture(5,0) );

            // space tr corner
            }else if(top == WALL
                 && bottom == SPACE 
                 && left == SPACE
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(5,1) );

            // space tri-section left
            }else if(top == WALL
                 && bottom == WALL 
                 && left == WALL
                 && right == SPACE){
                wall = new Wall( this->assets[WALL][0]->getTexture(6,3) );

            // space tri-section right
            }else if(top == WALL
                 && bottom == WALL 
                 && left == SPACE
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(6,1) );

            // space tri-section top
            }else if(top == WALL
                 && bottom == SPACE 
                 && left == WALL
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(6,0) );

            // space tri-section bottom
            }else if(top == SPACE
                 && bottom == WALL 
                 && left == WALL
                 && right == WALL){
                wall = new Wall( this->assets[WALL][0]->getTexture(6,2) );

            }else{
                wall = new Wall( this->assets[WALL][0]->getTexture(0,3) );
            }

            return wall;
        }
        Node* generateDoorNode(sf::Image* layout, int xPos, int yPos )
        {
            TILE top = this->checkColor( layout->getPixel( xPos, yPos - 1 ) );
            TILE left = this->checkColor( layout->getPixel( xPos - 1, yPos) );
            TILE right = this->checkColor( layout->getPixel( xPos + 1, yPos) );
            TILE bottom = this->checkColor( layout->getPixel( xPos, yPos + 1 ) );

            Node* door;
            if(top == FLOOR || bottom == FLOOR)
            {
                door = new Door( this->assets[DOOR][0]->getTexture(0,1), this->assets[FLOOR][0]->getTexture(0,0) );

            }else if( top == WALL || bottom  == WALL){
                door = new Door( this->assets[DOOR][0]->getTexture(0,0), this->assets[FLOOR][0]->getTexture(0,0) );

            }else if( top == SPACE || bottom == SPACE){
                door = new Door( this->assets[DOOR][0]->getTexture(1,1), this->assets[FLOOR][0]->getTexture(0,0) );

            }else if( left == SPACE || right == SPACE){
                door = new Door( this->assets[DOOR][0]->getTexture(1,1), this->assets[FLOOR][0]->getTexture(0,0) );
            }else{
                door = new Door( this->assets[DOOR][0]->getTexture(0,1), this->assets[FLOOR][0]->getTexture(0,0) );
            }
            return door;
        }
        Node* generateFloorNode(sf::Image* layout, int xPos, int yPos )
        {
            return new Floor( this->assets[FLOOR][0]->getTexture(0,0) );
        }
    };
}
#endif