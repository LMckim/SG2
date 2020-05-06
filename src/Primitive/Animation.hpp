#ifndef SG_PRIMITIVE_ANIMATION
#define SG_PRIMITIVE_ANIMATION
#include <vector>

#include <SFML/Graphics.hpp>

namespace SG::Primitive
{
    class Animation
    {
        public:
        // default constructor
        Animation() {};
        // copy constructor
        Animation( const Animation &original)
        {
            this->frames = original.frames;
            this->totalFrames = original.totalFrames;
        }
        // parameterized constructor
        Animation(sf::Image *spriteSheet, int sectionWidth = 16, int sectionHeight = 16, int yOffset = 0)
        {
            this->buildAnimation(spriteSheet, sectionWidth, sectionHeight, yOffset);
        }
        virtual ~Animation() {};
        sf::Texture* getTexture()
        {
            this->incrementAnimation();
            return this->frames[ this->currentFrame ];
        }
        sf::Texture* getTexture(int frame = 0)
        {
            return this->frames[ frame ];
        }
        sf::Texture* getTextureNoIncrement()
        {
            return this->frames[ this->currentFrame ];
        }
        uint16_t getTotalFrames()
        {
            return this->frames.size();
        }
        uint16_t getCurrentFrame()
        {
            return this->currentFrame;
        }
        sf::Texture* getRandomFrame()
        {
            int index = std::rand() % this->frames.size();
            return this->frames[index];
        }
        void addFrame(sf::Texture *tex)
        {
            this->frames.push_back( tex ); 
            this->totalFrames++;
        }
        protected:
        uint16_t currentFrame = 0;
        uint16_t totalFrames = 0;
        std::vector<sf::Texture*> frames;
        void incrementAnimation()
        {
            this->currentFrame++;
            if(this->currentFrame >= this->totalFrames) this->currentFrame = 0;
        }
        void buildAnimation(sf::Image *spriteSheet, int sectionWidth = 32, int sectionHeight = 32, int yOffset = 0)
        {
            int sections = spriteSheet->getSize().x / sectionWidth;
            this->totalFrames = sections;
            for(size_t section = 0; section < sections; section++)
            {
                sf::Texture *texture = new sf::Texture();
                texture->loadFromImage(
                    *spriteSheet,
                    sf::IntRect(
                    sf::Vector2i( section * sectionWidth, sectionHeight * yOffset),
                    sf::Vector2i( sectionWidth, sectionHeight ) )
                );
                this->frames.push_back( texture );
            }
        }
    };
}


#endif