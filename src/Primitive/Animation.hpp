#ifndef SG_PRIMITIVE_ANIMATION
#define SG_PRIMITIVE_ANIMATION

#include <vector>
#include <SFML/Graphics.hpp>

namespace SG::Primitive
{
    using std::vector;

    struct Animation
    {
        Animation() {}
        Animation( vector< sf::Texture* > frames ){ this->frames = frames; }

        int index = 0;
        vector< sf::Texture* > frames;

        int frameCount(){ return this->frames.size(); }
        void setFrames( vector< sf::Texture* > frames ){ this->frames = frames; }
        void addFrame( sf::Texture* frame ){ this->frames.push_back( frame ); }

        sf::Texture* getFrameByIndex( int index = 0 )
        {
            if(this->frames.size() <= index ){ return this->frames[ index ]; }
        }

        sf::Texture* getRandomFrame()
        {
            return this->frames[ std::rand() % this->frames.size() ];
        }
        
        sf::Texture* getNextFrame()
        {
            sf::Texture* frame = this->frames[ index ];
            this->index >= this->frames.size() ? this->index = 0 : this->index++;
            return frame;
        }
    };
}


#endif