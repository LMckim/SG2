#ifndef SG_MANAGER_EVENT
#define SG_MANAGER_EVENT

#include <SFML/Graphics.hpp>


#include <src/Primitive/Active.hpp>

using SG::Primitive::Active;

namespace SG::Manager
{
    class Event
    {
        public:
        Event( sf::RenderWindow* window) : window{ window } {}
        void handleEvents()
        {
            sf::Event event;
            while( this->window->pollEvent(event) )
            {
                if( event.type == sf::Event::Closed ) this->window->close(); 
            }
        }
        private:
        sf::RenderWindow* window;
        Active* active;
    };
}


#endif