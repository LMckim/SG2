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
        Animation( vector< sf::Texture* > _frames ) : frames{ _frames } {}

        uint16_t index = 0;
        vector< sf::Texture* > frames;

        uint16_t frameCount(){ return this->frames.size(); }
        void setIndex( uint16_t _index = 0){ this->index = _index; }
        void setFrames( vector< sf::Texture* > _frames ){ this->frames = _frames; }
        void addFrame( sf::Texture* _frame ){ this->frames.push_back( _frame ); }

        sf::Texture* getFrameByIndex( uint16_t _index = 0 )
        {
            if(this->frames.size() <= _index ){ return this->frames[ _index ]; }
            else return this->frames[0];
        }

        sf::Texture* getRandomFrame()
        {
            return this->frames[ std::rand() % this->frames.size() ];
        }
        
        sf::Texture* getNextFrame()
        {
            sf::Texture* frame = this->frames[ this->index ];
            this->index >= this->frames.size() ? this->index = 0 : this->index++;
            return frame;
        }
    };
}


#endif