#ifndef SG_GENERATOR
#define SG_GENERATOR

#include <map>
#include <iostream>
#include <src/Primitive/Generator.hpp>

#include <src/Node/Wall.hpp>
#include <src/Node/Floor.hpp>

namespace SG::Generator
{
    using std::vector;

    using SG::Manager::Object;
    using SG::Primitive::Node;
    using SG::Primitive::Generator;

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
            if( this->assets.find( TILE::SPACE ) == this->assets.end() 
            ||  this->assets.find( TILE::FLOOR ) == this->assets.end()
            ||  this->assets.find( TILE::WALL ) == this->assets.end() )
            { throw "Not enough assets to generate layout"; }

            int yS = layout->getSize().y;
            int xS = layout->getSize().x;
            for( size_t y=0; y < yS; y++)
            {   
                for(size_t x=0; x < xS; x++)
                {
                    // add nodes to our grid
                    switch( this->checkColor( layout->getPixel(x,y) ) )
                    {
                        case SPACE:

                        break;
                        case FLOOR:
                        
                        break;
                        case WALL:

                        break;
                        case DOOR:

                        break;
                        default:
                    }
                }
            }
        }
        protected:
        map< TILE, vector< sf::Texture* >> assets;
        vector< vector< Node* >> nodes;

        private:
        TILE checkColor(sf::Color &color)
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