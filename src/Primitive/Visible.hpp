#ifndef SG_PRIMITIVE_VISIBLE
#define SG_PRIMITIVE_VISIBLE

#include <SFML/Graphics.hpp>

// forward declaration
namespace SG::Manager{
    class Screen;
}

namespace SG::Primitive
{
    enum Z_LAYERS {
        BACKGROUND_DEEP = 0,
        BACKGROUND_GALAXY = 1,
        BACKGROUND_STAR = 2,
        BACKGROUND_SATELLITE = 3,

        SHIP_GRAPHIC = 10,
        SHIP_FLOOR = 11,
        SHIP_WALL = 12,

        CREW = 20,

        WINDOW_BASE = 50,
        WINDOW_BUTTON = 51,
        WINDOW_EXIT = 52,

        UI = 200
    };
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