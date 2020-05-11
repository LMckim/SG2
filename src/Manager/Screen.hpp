#ifndef SG_MANAGER_SCREEN
#define SG_MANAGER_SCREEN

#include <map>
#include <iostream>

#include <vector>

#include <SFML/System.hpp>
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

            this->clock = sf::Clock();
            this->time = this->clock.getElapsedTime();
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
                    itrZ++;
                }
                itr++;
            }

            // render any sf drawable primitives 
            for(auto &itr: this->sfPrimitives)
            {
                this->window->draw( *itr );
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
            this->window->setView( *this->view );
            this->time = this->clock.getElapsedTime();

            // std::cout << this->time.asMicroseconds() << std::endl;
            this->clock.restart();
        }
        
        void addVisible( sf::Drawable* drawable)
        {
            this->sfPrimitives.push_back( drawable );
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

        void removeVisible(sf::Drawable* drawable)
        {
            for( size_t i=0; i < this->sfPrimitives.size(); i++)
            {
                if(this->sfPrimitives[i] == drawable)
                {
                    this->sfPrimitives.erase( this->sfPrimitives.begin() + i );
                    break;
                }
            }
        }

        void removeVisible(Visible* visible)
        {
            // TODO: fix up here to use zlevel
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
        sf::Clock clock;
        sf::Time time;
        vector< sf::Drawable* > sfPrimitives;
        map< uint8_t, vector< Visible* > > visible;
        map< uint8_t, vector< Visible* > > ui;

    };
}
#endif