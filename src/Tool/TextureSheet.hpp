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
        TextureSheet( string _imagePath, uint16_t _sectionWidth = 16, uint16_t _sectionHeight = 16 )
        {
            this->imagePath = _imagePath;
            this->sectionWidth = _sectionWidth;
            this->sectionHeight = _sectionHeight;
        }

        void generateTextures()
        {
            sf::Image image;
            image.loadFromFile( this->imagePath );
            if( image.getSize().x % this->sectionWidth != 0 
            || image.getSize().y % this->sectionHeight != 0){
                throw "Section sizes are not consistent with image";    
            }

            uint16_t sectionsX = image.getSize().x / this->sectionWidth;
            uint16_t sectionsY = image.getSize().y / this->sectionHeight;

            for(size_t y = 0; y < sectionsY; y++ )
            {
                this->textureGrid.push_back( vector< sf::Texture* >() );
                for(size_t x = 0; x < sectionsX; x++ )
                {
                    sf::Texture* newTex = new sf::Texture();
                    newTex->loadFromImage( image, 
                        sf::IntRect(
                            x * this->sectionWidth,
                            y * this->sectionHeight, 
                            this->sectionWidth, 
                            this->sectionHeight
                    ));
                    this->textureGrid[y].push_back( newTex );
                }
            }

            this->generated = true;
        }

        sf::Texture* getTexture( uint16_t y, uint16_t x )
        {
            if( !this->generated ) this->generateTextures();

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
        
        Animation getAnimation( uint16_t y = 0 )
        {
            if( !this->generated ) this->generateTextures();
            
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
        uint16_t sectionWidth;
        uint16_t sectionHeight;
        string imagePath;
        vector< vector< sf::Texture* >> textureGrid;
    };
}
#endif