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
    class SpawnerWindow;

    class Button    :
        virtual protected Visible
    {
        friend class BaseWindow;
        friend class SpawnerWindow;

        public:
        Button(sf::Texture *buttonTexture) : btnTex{ buttonTexture }
        {
            this->zLevel = Z_LAYERS::WINDOW_BUTTON;
            this->sprite.setTexture( *this->btnTex );
            this->buildClickBox();
        }
        virtual ~Button()
        {
            delete btnTex;
            std::cout << "btn_del\n";
        }
        protected:
        const sf::Color CLICK_BOX_COLOR = sf::Color::Red;
        const float CLICK_BOX_OUTLINE_THICKNESS = 0.9f;
        bool showClickBox = true;
        sf::Texture *btnTex;
        sf::RectangleShape clickBox;
        virtual void onClick() { std::cout << "clicked " << this << std::endl; };
        virtual void scale(float scaleFactor)
        {
            this->sprite.setScale(
                this->sprite.getScale().x * scaleFactor,
                this->sprite.getScale().y * scaleFactor
            );
            this->clickBox.setSize(
                sf::Vector2f(
                    this->sprite.getGlobalBounds().width,
                    this->sprite.getGlobalBounds().height
                )
            );
            this->clickBox.setPosition( this->sprite.getPosition() );
        }
        virtual void setPosition(sf::Vector2f pos)
        {
            this->sprite.setPosition(pos);
            this->clickBox.setPosition( this->sprite.getPosition() );
        }
        virtual void buildClickBox()
        {
            this->clickBox.setFillColor( sf::Color::Transparent );
            this->clickBox.setOutlineColor( CLICK_BOX_COLOR );
            this->clickBox.setOutlineThickness( CLICK_BOX_OUTLINE_THICKNESS );
            this->clickBox.setSize(
                sf::Vector2f(
                    this->sprite.getGlobalBounds().width,
                    this->sprite.getGlobalBounds().height
                )
            );
            this->clickBox.setPosition( this->sprite.getPosition() );
        }
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->sprite );
            if(this->showClickBox) target.draw( this->clickBox );
        }
    };
}


#endif