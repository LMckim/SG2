#ifndef SG_MANAGER_EVENT
#define SG_MANAGER_EVENT

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Object.hpp>
#include <src/Primitive/Active.hpp>

namespace SG::Manager
{
    using SG::Primitive::Active;
    using SG::Manager::Screen;
    using SG::Manager::Object;
    class Event
    {
        public:
        Event( Screen* screenM ) : screenM{ screenM } {}
        Event( Screen* screenM, Object* objectM ) : screenM{ screenM}, objectM{ objectM } {}
        void handleEvents()
        {
            sf::RenderWindow* window = this->screenM->window;
            sf::View* view = this->screenM->view;

            sf::Event event;
            while( window->pollEvent(event) )
            {
                if( event.type == sf::Event::Closed ) window->close();
                // handles zooming in and out
                else if( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta == 1 && this->scrolls < 30 )
                {
                    this->scrolls++;
                    view->zoom( this->zoomInPerc );
                }else if( event.type == sf::Event::MouseWheelMoved && event.mouseWheel.delta == -1 && this->scrolls > -10 )
                {
                    this->scrolls--;
                    view->zoom( this->zoomOutPerc );

                // handles shifting around the ship map using the middle mouse button
                }else if( sf::Mouse::isButtonPressed( sf::Mouse::Middle ) ){
                    
                    if( sf::Mouse::getPosition( *window ).x > this->prevMousePos.x && this->shiftedAmount.x < 100 ){
                        view->move( this->shiftAmount, 0);
                        this->shiftedAmount.x++;
                    }else if( sf::Mouse::getPosition( *window ).x < this->prevMousePos.x && this->shiftedAmount.x > -100 ){
                        view->move( -this->shiftAmount, 0);
                        this->shiftedAmount.x--;
                    }
                    
                    if( sf::Mouse::getPosition( *window ).y > this->prevMousePos.y && this->shiftedAmount.x < 100 ){
                        view->move( 0, this->shiftAmount);
                        this->shiftedAmount.y++;
                    }else if( sf::Mouse::getPosition( *window ).y < this->prevMousePos.y && this->shiftedAmount.x > -100 ){
                        view->move( 0, -this->shiftAmount);
                        this->shiftedAmount.y--;
                    }
                }else if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) ){
                    sf::Vector2f mPos = window->mapPixelToCoords(sf::Mouse::getPosition( *window ));
                    this->objectM->checkClicked( mPos );
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
        Object* objectM;
        Active* active;
    };
}


#endif