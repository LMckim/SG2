#ifndef SG_GENERATOR
#define SG_GENERATOR

#include <map>
#include <iostream>
#include <src/Primitive/Generator.hpp>

#include <src/Node/Wall.hpp>
#include <src/Node/Floor.hpp>
#include <src/Node/Door.hpp>
#include <src/Node/Space.hpp>

namespace SG::Generator
{
    using std::vector;

    using SG::Manager::Object;
    using SG::Primitive::Node;
    using SG::Primitive::Generator;

    using SG::Node::Floor;
    using SG::Node::Wall;
    using SG::Node::Door;
    using SG::Node::Space;

    class Layout :
        protected Generator
    {
        public:
        const int TILE_SIZE = 16;
        enum TILE { SPACE, FLOOR, WALL, DOOR };
        
        Layout(Object* objectM) : Generator::Generator( objectM ) {}
        ~Layout() {}

        void addAsset(TILE tile, sf::Texture* asset)
        {
            this->assets[tile].push_back(asset);
        }

        void generateLayout(sf::Image* layout)
        {
            if( this->assets.find( TILE::DOOR ) == this->assets.end() 
            ||  this->assets.find( TILE::FLOOR ) == this->assets.end()
            ||  this->assets.find( TILE::WALL ) == this->assets.end() )
            { throw "Not enough assets to generate layout"; }

            int yS = layout->getSize().y;
            int xS = layout->getSize().x;

            for( size_t y=0; y < yS; y++)
            {   
                this->nodes.push_back(vector< Node* >());
                for(size_t x=0; x < xS; x++)
                {
                    // add nodes to our grid
                    Node* newNode;
                    switch( this->checkColor( layout->getPixel(x,y) ))
                    {
                        case FLOOR:
                            newNode = new Floor(this->assets[ FLOOR ][ (std::rand() % this->assets[ FLOOR ].size()) ]);
                        break;
                        case WALL:
                            newNode = new Wall(this->assets[ WALL ][ (std::rand() % this->assets[ WALL ].size()) ]);
                        break;
                        case DOOR:
                            newNode = new Door(this->assets[ DOOR ][ (std::rand() % this->assets[ DOOR ].size()) ]);
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
        }
        protected:
        map< TILE, vector< sf::Texture* >> assets;
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
            }else if(color.r == 0 && color.g == 255 && color.b == 255 && color.a == 255){
                return DOOR;
            // default to space node
            }else{
                return SPACE;
            }
        }
    };
}
#endif