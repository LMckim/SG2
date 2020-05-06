#ifndef SG_PRIMITIVE_VISIBLE
#define SG_PRIMITIVE_VISIBLE

#include <SFML/Graphics.hpp>

// forward declaration
namespace SG::Manager{
    class Screen;
}

namespace SG::Primitive
{
    class Visible
    {
        friend class SG::Manager::Screen;
        public:
        uint8_t zLevel = 0;
        virtual ~Visible() {}
        protected:
        sf::Sprite sprite;
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->sprite );
        }
    };
}


#endif