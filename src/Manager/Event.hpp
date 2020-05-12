#ifndef SG_MANAGER_EVENT
#define SG_MANAGER_EVENT

#include <map>
#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Object.hpp>
#include <src/Primitive/Active.hpp>

namespace SG::Manager
{
    using std::map;
    using SG::Primitive::Active;
    using SG::Manager::Screen;
    using SG::Manager::Object;
    class Event
    {
        public:
        Event( Screen* screenM, Object* objectM ) : screenM{ screenM }, objectM{ objectM } 
        {
            this->selectionbox.setOutlineThickness( 1.f );
            this->selectionbox.setOutlineColor( sf::Color::Green );
            this->selectionbox.setFillColor( sf::Color::Transparent );

            // register keys
            this->keyDelays[ sf::Keyboard::Space ] = 0;
        }
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
                    
                    if( sf::Mouse::getPosition( *window ).x > this->prevMousePos.x && this->shiftedAmount.x < 200 ){
                        view->move( this->shiftAmount, 0);
                        this->shiftedAmount.x++;
                    }else if( sf::Mouse::getPosition( *window ).x < this->prevMousePos.x && this->shiftedAmount.x > -200 ){
                        view->move( -this->shiftAmount, 0);
                        this->shiftedAmount.x--;
                    }
                    
                    if( sf::Mouse::getPosition( *window ).y > this->prevMousePos.y && this->shiftedAmount.y < 100 ){
                        view->move( 0, this->shiftAmount);
                        this->shiftedAmount.y++;
                    }else if( sf::Mouse::getPosition( *window ).y < this->prevMousePos.y && this->shiftedAmount.y > -100 ){
                        view->move( 0, -this->shiftAmount);
                        this->shiftedAmount.y--;
                    }
                }
                // left click actions
                if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) ){
                    // handle drawing of the selection box
                    if(leftHeld == false)
                    {
                        leftHeld = true;
                        this->selectionOrigin = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );
                        this->selectionbox.setPosition( selectionOrigin );

                        this->screenM->addVisible( &this->selectionbox );
                    }else{
                        this->selectionbox.setSize(
                            sf::Vector2f(
                                window->mapPixelToCoords( sf::Mouse::getPosition( *window ) ).x - selectionOrigin.x,
                                window->mapPixelToCoords( sf::Mouse::getPosition( *window ) ).y - selectionOrigin.y
                            )
                        );
                    }

                    sf::Vector2f mPos = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );

                    // hmmmm
                    if(this->selectionbox.getSize().x < SELECTION_THRESHOLD && this->selectionbox.getSize().y < SELECTION_THRESHOLD)
                    {
                        this->objectM->quickClick( mPos );
                    }
                    

                }else{
                    leftHeld = false;
                    if(this->selectionbox.getSize().x > SELECTION_THRESHOLD && this->selectionbox.getSize().y > SELECTION_THRESHOLD)
                    {
                        this->objectM->processSelectionBox( &this->selectionbox );
                    }
        
                    this->selectionbox.setSize( sf::Vector2f(0,0) );
                    this->screenM->removeVisible( &this->selectionbox );
                }

                if( sf::Mouse::isButtonPressed( sf::Mouse::Right ) ){
                    sf::Vector2f mPos = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );
                    this->objectM->rightClicked( mPos );
                }
                // SPACE KEY PRESSED
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) && this->keyDelays[ sf::Keyboard::Space] == 0)
                {
                    this->keyDelays[ sf::Keyboard::Space ] = KEY_DELAY_FRAMES;
                    
                    std::cout << "pause\n";
                    this->objectM->togglePause();
                }

            }
            this->prevMousePos = sf::Mouse::getPosition( *window );
            this->decrementKeyDelays();
        }
        private:
        const uint8_t KEY_DELAY_FRAMES = 15;
        const float SELECTION_THRESHOLD = 1.f;
        bool leftHeld = true;
        sf::Vector2f selectionOrigin;
        sf::RectangleShape selectionbox;

        const float shiftAmount = 5;
        const float zoomInPerc = 0.95f;
        const float zoomOutPerc = 1.05f;

        int8_t scrolls = 0;
        sf::Vector2i prevMousePos;
        sf::Vector2i shiftedAmount = sf::Vector2i(0,0);
        Screen* screenM;
        Object* objectM;
        Active* active;

        map < sf::Keyboard::Key, uint8_t > keyDelays;

        void decrementKeyDelays()
        {
            for( auto &itr : this->keyDelays )
            {
                if(itr.second > 0) itr.second--;
            }
        }

    };
}


#endif