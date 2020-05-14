#ifndef SG_MANAGER_EVENT
#define SG_MANAGER_EVENT

#include <map>
#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Object.hpp>
#include <src/Primitive/Active.hpp>

// sub modules
namespace SG::Manager
{
    using std::map;
    using SG::Primitive::Active;
    using SG::Manager::Screen;
    using SG::Manager::Object;
    class Event
    {
        public:
        Event(Screen* screenM, Object* objectM ) : screenM{ screenM }, objectM{ objectM } 
        {
            this->selectionbox.setOutlineThickness( 1.f );
            this->selectionbox.setOutlineColor( sf::Color::Green );
            this->selectionbox.setFillColor( sf::Color::Transparent );

            // register mouse buttons
            this->m_held[ sf::Mouse::Left ] = false;
            this->m_held[ sf::Mouse::Right ] = false;
            this->m_held[ sf::Mouse::Middle ] = false;

            // register keys
            // held 
            this->k_held[ sf::Keyboard::Space ] = false;
            //
            this->keyDelays[ sf::Keyboard::Space ] = 0; // pausing
            this->keyDelays[ sf::Keyboard::Tab ] = 0; // changing between map and in-game
        }
        void handleEvents()
        {
            sf::RenderWindow* window = this->screenM->window;
            sf::View* view = this->screenM->view;

            sf::Event event;
            while( window->pollEvent(event) )
            {
                // ********* MAJOR EVENTS ************
                // TODO: implement proper shutdown procedure
                if( event.type == sf::Event::Closed ) window->close();

                // ********* MOUSE ACTIONS ***********
                // mouse wheel
                if( event.type == sf::Event::MouseWheelMoved) { this->mouseWheel( &event ); }
                // first click
                if(event.type == sf::Event::MouseButtonPressed){
                    // left
                    if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && this->m_held[ sf::Mouse::Left ] == false ){
                        this->m_held[ sf::Mouse::Left ] = true;
                        this->leftClick( window );
                        std::cout << "click\n";
                    }
                    // right
                    if( sf::Mouse::isButtonPressed( sf::Mouse::Right ) && this->m_held[ sf::Mouse::Right ] == false ){
                        this->m_held[ sf::Mouse::Right ] = true;
                        this->rightClick( window );
                    }
                    // middle
                    if( sf::Mouse::isButtonPressed( sf::Mouse::Middle ) && this->m_held[ sf::Mouse::Middle ] == false ){
                        this->m_held[ sf::Mouse::Middle ] = true;
                        this->middleClick( window, view );
                    }

                // released
                }else if(event.type == sf::Event::MouseButtonReleased){
                    // left
                    if( !sf::Mouse::isButtonPressed( sf::Mouse::Left )  && this->m_held[ sf::Mouse::Left ] == true){
                        this->m_held[ sf::Mouse::Left ] = false;
                        std::cout << "released\n";
                    }
                    // right
                    if( !sf::Mouse::isButtonPressed( sf::Mouse::Right )  && this->m_held[ sf::Mouse::Right ] == true){
                        this->m_held[ sf::Mouse::Right ] = false;
                        std::cout << "released\n";
                    }
                    // middle
                    if( !sf::Mouse::isButtonPressed( sf::Mouse::Middle )  && this->m_held[ sf::Mouse::Middle ] == true){
                        this->m_held[ sf::Mouse::Middle ] = false;
                        std::cout << "released\n";
                    }


                // held
                }else{
                    // left
                    if( this->m_held[ sf::Mouse::Left ] == true){
                        this->leftClick( window );
                    }
                    // right
                    if( this->m_held[ sf::Mouse::Right ] == true){
                    }
                    // middle
                    if( this->m_held[ sf::Mouse::Middle ] == true){
                        this->middleClick( window, view );
                    }
                }
                
                // SPACE KEY PRESSED
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) && this->keyDelays[ sf::Keyboard::Space ] == 0)
                {
                    this->keyDelays[ sf::Keyboard::Space ] = KEY_DELAY_FRAMES;
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

        const float SHIFT_AMOUNT = 5;
        const float ZOOM_IN_PERCENT = 0.95f;
        const float ZOOM_OUT_PERCENT = 1.05f;

        int8_t scrolls = 0;
        sf::Vector2i prevMousePos;
        sf::Vector2i shiftedAmount = sf::Vector2i(0,0);
        Screen* screenM;
        Object* objectM;
        Active* active;

        map < sf::Mouse::Button, bool > m_held;
        map <sf::Keyboard::Key, bool > k_held;
        map < sf::Keyboard::Key, uint8_t > keyDelays;

        void decrementKeyDelays()
        {
            for( auto &itr : this->keyDelays )
            {
                if(itr.second > 0) itr.second--;
            }
        }
        void mouseWheel(const sf::Event* event)
        {
                if(event->mouseWheel.delta == 1 && this->scrolls < 30 )
                {
                    this->scrolls++;
                    this->screenM->zoom( ZOOM_IN_PERCENT );
                }else if(event->mouseWheel.delta == -1 && this->scrolls > -10 )
                {
                    this->scrolls--;
                    this->screenM->zoom( ZOOM_OUT_PERCENT );
                }
        }
        void middleClick(const sf::RenderWindow* window, sf::View* view)
        {
            if( sf::Mouse::getPosition( *window ).x > this->prevMousePos.x && this->shiftedAmount.x < 200 ){
                view->move( SHIFT_AMOUNT, 0);
                this->shiftedAmount.x++;
            }else if( sf::Mouse::getPosition( *window ).x < this->prevMousePos.x && this->shiftedAmount.x > -200 ){
                // negative shift
                view->move( -SHIFT_AMOUNT, 0);
                this->shiftedAmount.x--;
            }
            
            if( sf::Mouse::getPosition( *window ).y > this->prevMousePos.y && this->shiftedAmount.y < 100 ){
                view->move( 0, this->SHIFT_AMOUNT);
                this->shiftedAmount.y++;
            }else if( sf::Mouse::getPosition( *window ).y < this->prevMousePos.y && this->shiftedAmount.y > -100 ){
                view->move( 0, -this->SHIFT_AMOUNT);
                this->shiftedAmount.y--;
            }
        }
        void leftDown()
        {
            
        }
        void leftClick(const sf::RenderWindow* window )
        {
            if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) ){
                // handle drawing of the selection box
                if( leftHeld == false && this->objectM->dragging == false )
                {
                    leftHeld = true;
                    this->selectionOrigin = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );
                    this->selectionbox.setPosition( selectionOrigin );

                    this->screenM->addVisible( &this->selectionbox );
                }else if( this->objectM->dragging == false ){
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
                this->objectM->dragging = false;
                if(this->selectionbox.getSize().x > SELECTION_THRESHOLD && this->selectionbox.getSize().y > SELECTION_THRESHOLD)
                {
                    this->objectM->processSelectionBox( &this->selectionbox );
                }
    
                this->selectionbox.setSize( sf::Vector2f(0,0) );
                this->screenM->removeVisible( &this->selectionbox );
            }
        }
        void rightClick(const sf::RenderWindow* window )
        {
            sf::Vector2f mPos = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );
            this->objectM->rightClicked( mPos );
        }
    };
}


#endif