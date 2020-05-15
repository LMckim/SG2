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
        Event(Screen* _screenM, Object* _objectM ) : screenM{ _screenM }, objectM{ _objectM } 
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
            this->k_held[ sf::Keyboard::R ] = false;
            // delays
            this->keyDelays[ sf::Keyboard::R ] = 0;
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
                // get the mouse position
                sf::Vector2f mPos = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );

                // mouse wheel
                if( event.type == sf::Event::MouseWheelMoved) { this->mouseWheel( &event ); }
                // first click
                if(event.type == sf::Event::MouseButtonPressed)
                {
                    // left
                    if( sf::Mouse::isButtonPressed( sf::Mouse::Left ) && this->m_held[ sf::Mouse::Left ] == false ){
                        this->m_held[ sf::Mouse::Left ] = true;
                        this->leftClick( window, mPos );
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
                        this->leftUp();
                    }
                    // right
                    if( !sf::Mouse::isButtonPressed( sf::Mouse::Right )  && this->m_held[ sf::Mouse::Right ] == true){
                        this->m_held[ sf::Mouse::Right ] = false;
                    }
                    // middle
                    if( !sf::Mouse::isButtonPressed( sf::Mouse::Middle )  && this->m_held[ sf::Mouse::Middle ] == true){
                        this->m_held[ sf::Mouse::Middle ] = false;
                    }


                // held
                }else{
                    // left
                    if( this->m_held[ sf::Mouse::Left ] == true){
                        this->leftHeld( window, mPos );
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
                // TODO: implement the same funcitonality as above
                if( sf::Keyboard::isKeyPressed( sf::Keyboard::Space ) && this->keyDelays[ sf::Keyboard::Space ] == 0)
                {
                    this->keyDelays[ sf::Keyboard::Space ] = KEY_DELAY_FRAMES;
                    this->objectM->togglePause();
                }
                if(event.type == sf::Event::KeyPressed)
                {
                    if(sf::Keyboard::isKeyPressed( sf::Keyboard::R ) && this->k_held[ sf::Keyboard::R ] == false)
                    {
                        this->pressed_R( event );
                    }
                }
                

            }
            this->prevMousePos = sf::Mouse::getPosition( *window );
            this->decrementKeyDelays();
        }
        private:
        const uint8_t KEY_DELAY_FRAMES = 15;
        const float SELECTION_THRESHOLD = 1.f;
        sf::Vector2f selectionOrigin;
        sf::RectangleShape selectionbox;

        const float SHIFT_AMOUNT = 5;
        const float SHIFT_X_MAX = 200;
        const float SHIFT_Y_MAX = 100;
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
        // ********** MOUSE ************
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
            if( sf::Mouse::getPosition( *window ).x > this->prevMousePos.x && this->shiftedAmount.x < SHIFT_X_MAX ){
                view->move( SHIFT_AMOUNT, 0);
                this->shiftedAmount.x++;
            }else if( sf::Mouse::getPosition( *window ).x < this->prevMousePos.x && this->shiftedAmount.x >  -SHIFT_X_MAX ){
                // negative shift
                view->move( -SHIFT_AMOUNT, 0);
                this->shiftedAmount.x--;
            }
            
            if( sf::Mouse::getPosition( *window ).y > this->prevMousePos.y && this->shiftedAmount.y < SHIFT_Y_MAX ){
                view->move( 0, this->SHIFT_AMOUNT);
                this->shiftedAmount.y++;
            }else if( sf::Mouse::getPosition( *window ).y < this->prevMousePos.y && this->shiftedAmount.y > -SHIFT_Y_MAX ){
                view->move( 0, -this->SHIFT_AMOUNT);
                this->shiftedAmount.y--;
            }
        }

        void leftClick(const sf::RenderWindow* window, const sf::Vector2f mPos )
        {
            this->selectionOrigin = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );
            this->objectM->leftClick( mPos );
        }
        void leftHeld( const sf::RenderWindow* window, const sf::Vector2f mPos )
        {
            // if the objectM confirms we are not currently dragging an object
            if(this->objectM->dragging == false)
            {
                // draw, set and expand the selection box
                this->screenM->addVisible( &this->selectionbox );
                this->selectionbox.setPosition( selectionOrigin );

                this->selectionbox.setSize(
                    sf::Vector2f(
                        window->mapPixelToCoords( sf::Mouse::getPosition( *window ) ).x - selectionOrigin.x,
                        window->mapPixelToCoords( sf::Mouse::getPosition( *window ) ).y - selectionOrigin.y
                    )
                );
            // if were actively engaged in draggin then update
            }else if(this->objectM->dragging == true){
                this->objectM->leftDrag( mPos );
            }
        }
        void leftUp()
        {
            // tell the object Manager were not dragging anymore
            if(this->objectM->dragging)
            { 
                this->objectM->clearDrag();
            // if the selection box is big enough then process it
            }else if(this->selectionbox.getSize().x > SELECTION_THRESHOLD && this->selectionbox.getSize().y > SELECTION_THRESHOLD)
            {
                this->objectM->processSelectionBox( &this->selectionbox );
            }
            // either way reset the selection box
            this->selectionbox.setSize( sf::Vector2f(0,0) );
            this->screenM->removeVisible( &this->selectionbox );
        }
        void rightClick(const sf::RenderWindow* window )
        {
            sf::Vector2f mPos = window->mapPixelToCoords( sf::Mouse::getPosition( *window ) );
            this->objectM->rightClicked( mPos );
        }
        // ********** KEYBOARD ************
        void pressed_R(sf::Event event)
        {
            if(this->objectM->dragging == true)
            {
                this->objectM->DraggedObj->handleInput( event );
            }
        }
    };
}


#endif