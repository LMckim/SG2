#ifndef SG_WINDOW_BUTTON_TEXT
#define SG_WINDOW_BUTTON_TEXT

#include <vector>
#include <string>

#include <src/Window/Button.hpp>

namespace SG::Window
{
    using std::string;
    using SG::Window::Button;

    
    class TextButton    :
        virtual protected Button
    {
        friend class BaseWindow;

        public:
        TextButton(sf::Texture* buttonTexture, sf::Font* _font, string _text) : Button( buttonTexture )
        {
            this->font = _font;
            this->text.setFont(*this->font);
            this->text.setScale(0.5, 0.5);
            this->text.setString( _text );
            this->text.setOrigin(
                this->text.getGlobalBounds().width / 2,
                this->text.getGlobalBounds().height / 2
            );
            this->positionText();
        }
        virtual ~TextButton()
        {
        }
        protected:
        sf::Font* font;
        sf::Text text;
        void setPosition(sf::Vector2f pos) override
        {
            Button::setPosition( pos );
            this->positionText();
        }
        virtual void scale(float scaleFactor) override
        {
            Button::scale( scaleFactor );
            this->text.setScale( this->text.getScale() * scaleFactor ); 
        }
        void positionText()
        {
            // TODO: text positioning needs some work, isnt centered
            this->text.setPosition( 
                this->sprite.getPosition().x + (this->sprite.getGlobalBounds().width / 2),
                this->sprite.getPosition().y + (this->sprite.getGlobalBounds().height / 2)
            );
        }
        virtual void draw(sf::RenderTarget& target)
        {
            Button::draw( target );
            target.draw( this->text );
        }
    };
}


#endif