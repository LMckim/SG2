#ifndef SG_MANAGER_SCREEN
#define SG_MANAGER_SCREEN

#include <map>
#include <vector>

#include <SFML/Graphics.hpp>
#include <src/Primitive/Visible.hpp>

using std::map;
using std::vector;
using SG::Primitive::Visible;

namespace SG::Manager
{
    class Screen
    {
        public:
        sf::RenderWindow *window;
        sf::View *view;
        Screen()
        {
            this->window = new sf::RenderWindow( sf::VideoMode(1200,800), "SPACE GAME 2" );
            this->window->setFramerateLimit( 120 );
            this->view = new sf::View( 
                sf::FloatRect( 
                    0.f,
                    0.f,
                    this->window->getSize().x, 
                    this->window->getSize().y
                ) 
            );
            this->window->setView( *this->view );
        }
        void render()
        {
            // clear the window
            this->window->clear();
            // render the regular objects
            this->window->setView( *this->view );
            auto itr = this->visible.begin();
            while( itr != this->visible.end() )
            {
                auto itrZ = itr->second.begin();
                while( itrZ != itr->second.end() )
                {
                    (*itrZ)->draw( *this->window );
                }
                itr++;
            }
            // render the ui
            this->window->setView( this->window->getDefaultView() );
            itr = this->ui.begin();
            while( itr != this->ui.end() )
            {
                auto itrZ = itr->second.begin();
                while( itrZ != itr->second.end() )
                {
                    (*itrZ)->draw( *this->window );
                }
                itr++;
            }
            
            this->window->display();
        }
        void addVisible(Visible* visible)
        {
            if( this->visible.find( visible->zLevel ) != this->visible.end())
            {
                this->visible[ visible->zLevel ].push_back( visible );
            }else{
                this->visible[ visible->zLevel ] = vector< Visible* >();
                this->visible[ visible->zLevel ].push_back( visible );
            }
        }
        void removeVisible(Visible* visible)
        {
            auto itr = this->visible.begin();
            while( itr != this->visible.end() )
            {
                auto itrZ = itr->second.begin();
                while( itrZ != itr->second.end() )
                {
                    if((*itrZ) == visible)
                    {
                        itrZ = itr->second.erase( itrZ );
                    }else itrZ++;
                }
                itr++;
            }
        }
        private:
        map< uint8_t, vector< Visible* > > visible;
        map< uint8_t, vector< Visible* > > ui;

    };
}
#endif