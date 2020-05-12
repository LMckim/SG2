#ifndef SG_WINDOW_BUTTON
#define SG_WINDOW_BUTTON

#include <vector>
#include <string>

#include <src/Primitive/Visible.hpp>

namespace SG::Window
{
    using std::string;
    using SG::Primitive::Z_LAYERS;
    using SG::Primitive::Visible;
    // forward declaration
    class BaseWindow;

    class Button    :
        virtual protected Visible
    {
        friend class BaseWindow;

        public:
        Button(uint8_t zLevel, sf::Texture* buttonTexture, sf::Font* font, string text) : font{ font }
        {
            this->zLevel = Z_LAYERS::WINDOW_BUTTON;
            this->sprite.setTexture( *buttonTexture );
            this->text.setString( text );
            this->text.setPosition( 
                this->sprite.getTexture()->getSize().x / 2,
                this->sprite.getTexture()->getSize().y / 2
            );
        }
        protected:
        sf::Font* font;
        sf::Text text;
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->sprite );
            target.draw( this->text );
        }
    };
}


#endif