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
        Button(sf::Texture *buttonTexture) : btnTex{ buttonTexture }
        {
            this->zLevel = Z_LAYERS::WINDOW_BUTTON;
            this->sprite.setTexture( *this->btnTex );
        }
        virtual ~Button()
        {
            delete btnTex;
        }
        protected:
        sf::Texture *btnTex;
        virtual void scale(float scaleFactor)
        {
            this->sprite.setScale(
                    this->sprite.getScale().x * scaleFactor,
                    this->sprite.getScale().y * scaleFactor
                );
        }
        virtual void setPosition(sf::Vector2f pos)
        {
            this->sprite.setPosition(pos);
        }
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->sprite );
        }
    };
}


#endif