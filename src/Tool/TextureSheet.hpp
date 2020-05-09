#ifndef SG_TOOL_TEXTURE_SHEET
#define SG_TOOL_TEXTURE_SHEET

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include <src/Primitive/Animation.hpp>

namespace SG::Tool
{
    using std::string;
    using std::vector;
    using SG::Primitive::Animation;

    class TextureSheet
    {
        public:
        TextureSheet( string imagePath, int sectionWidth = 16, int sectionHeight = 16 )
        {
            this->imagePath = imagePath;
            this->sectionWidth = sectionWidth;
            this->sectionHeight = sectionHeight;
        }

        void generateTextures()
        {
            sf::Image image;
            image.loadFromFile( this->imagePath );
            if( image.getSize().x % this->sectionWidth != 0 
            || image.getSize().y % this->sectionHeight != 0){
                throw "Section sizes are not consistent with image";    
            }

            int sectionsX = image.getSize().x / this->sectionWidth;
            int sectionsY = image.getSize().y / this->sectionHeight;

            for(size_t y = 0; y < sectionsY; y += this->sectionWidth )
            {
                this->textureGrid.push_back( vector< sf::Texture* >() );
                for(size_t x = 0; x < sectionsX; x += this->sectionHeight )
                {
                    sf::Texture* newTex = new sf::Texture();
                    newTex->loadFromImage( image, 
                        sf::IntRect(
                            x, y, this->sectionWidth, this->sectionHeight
                    ));
                    this->textureGrid[y].push_back( newTex );
                }
            }

            this->generated = true;
        }

        sf::Texture* getTexture( int x, int y )
        {
            if( this->textureGrid.size() >= y )
            {
                if( this->textureGrid[ y ].size() >= x )
                {
                    return this->textureGrid[ y ][ x ];
                }
            }
            // failover
            return nullptr;
        }
        Animation getAnimation( int y = 0 )
        {
            if( this->textureGrid.size() >= y )
            {
                Animation animation;
                for(size_t x = 0; x < this->textureGrid[y].size(); x++)
                {
                    animation.addFrame( this->textureGrid[y][x] );
                }
                return animation;
            }else return Animation();
        }


        private:
        bool generated = false;
        int sectionWidth;
        int sectionHeight;
        string imagePath;
        vector< vector< sf::Texture* >> textureGrid;
    };
}
#endif