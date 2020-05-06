#ifndef SG_MANAGER_EVENT
#define SG_MANAGER_EVENT

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Primitive/Active.hpp>
using SG::Primitive::Active;

namespace SG::Manager
{
    using SG::Manager::Screen;
    class Event
    {
        public:
        Event( Screen* screenM) : screenM{ screenM } {}
        void handleEvents()
        {
            sf::RenderWindow* window = this->screenM->window;
            sf::View* view = this->screenM->view;

            sf::Event event;
            while( window->pollEvent(event) )
            {
                if( event.type == sf::Event::Closed ) window->close();
                else if(event.mouseWheel.delta == 1 && this->scrolls < 30)
                {
                    this->scrolls++;
                    view->zoom( this->zoomInPerc );
                }else if(event.mouseWheel.delta == -1 && this->scrolls > -10)
                {
                    this->scrolls--;
                    view->zoom( this->zoomOutPerc );
                }else if(sf::Mouse::isButtonPressed( sf::Mouse::Middle ) ){
                    
                    // handles shifting around the ship map
                    if(sf::Mouse::getPosition( *window ).x > this->prevMousePos.x && this->shiftedAmount.x < 100){
                        view->move( this->shiftAmount, 0);
                        this->shiftedAmount.x++;
                    }else if(sf::Mouse::getPosition( *window ).x < this->prevMousePos.x && this->shiftedAmount.x > -100){
                        view->move( -this->shiftAmount, 0);
                        this->shiftedAmount.x--;
                    }else if(sf::Mouse::getPosition( *window ).y > this->prevMousePos.y && this->shiftedAmount.x < 100){
                        view->move( 0, this->shiftAmount);
                        this->shiftedAmount.y++;
                    }else if(sf::Mouse::getPosition( *window ).y < this->prevMousePos.y && this->shiftedAmount.x > -100){
                        view->move( 0, -this->shiftAmount);
                        this->shiftedAmount.y--;
                    }
                }
            }
            this->prevMousePos = sf::Mouse::getPosition( *window );
        }
        private:
        const float shiftAmount = 5;
        const float zoomInPerc = 0.95f;
        const float zoomOutPerc = 1.05f;

        int8_t scrolls = 0;
        sf::Vector2i prevMousePos;
        sf::Vector2i shiftedAmount = sf::Vector2i(0,0);
        Screen* screenM;
        Active* active;
    };
}


#endif