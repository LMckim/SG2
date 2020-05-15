#ifndef SG_MANAGER_SCREEN
#define SG_MANAGER_SCREEN

#include <map>
#include <iostream>

#include <vector>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Visible/Reactive.hpp>

namespace SG::Manager
{
    using std::map;
    using std::vector;
    using SG::Primitive::Visible;
    using SG::Primitive::Reactive;

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
            for(auto &itr_p: this->sfPrimitives)
            {
                this->window->draw( *itr_p );
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
                    itrZ++;
                }
                itr++;
            }
            
            this->window->display();
            this->window->setView( *this->view );

            // fps counter
            this->time = this->clock.getElapsedTime();
            if(this->time.asSeconds() >= 1)
            {
                this->window->setTitle("SG2 Render Time:"+std::to_string(this->fps));
                this->fps = 0;
                this->clock.restart();

            }else this->fps++;

        }
        
        void addVisible( sf::Drawable* drawable)
        {
            this->sfPrimitives.push_back( drawable );
        }

        void addVisible(Visible* _visible)
        {
            if( this->visible.find( _visible->zLevel ) != this->visible.end())
            {
                this->visible[ _visible->zLevel ].push_back( _visible );
            }else{
                this->visible[ _visible->zLevel ] = vector< Visible* >();
                this->visible[ _visible->zLevel ].push_back( _visible );
            }
        }
        
        void addUI(Visible* _visible)
        {
            if( this->ui.find( _visible->zLevel ) != this->ui.end())
            {
                this->ui[ _visible->zLevel ].push_back( _visible );
            }else{
                this->ui[ _visible->zLevel ] = vector< Visible* >();
                this->ui[ _visible->zLevel ].push_back( _visible );
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

        void removeVisible(Visible* _visible)
        {
            // TODO: fix up here to use zlevel
            auto itr = this->visible.begin();
            while( itr != this->visible.end() )
            {
                auto itrZ = itr->second.begin();
                while( itrZ != itr->second.end() )
                {
                    if((*itrZ) == _visible)
                    {
                        itrZ = itr->second.erase( itrZ );
                    }else itrZ++;
                }
                itr++;
            }
        }

        void zoom( float zoomFactor )
        {
            this->view->zoom( zoomFactor );
            auto itrZ = this->visible.begin();
            while( itrZ != this->visible.end() )
            {
                auto itrV = itrZ->second.begin();
                while( itrV != itrZ->second.end() )
                {
                    if(Reactive* reactElement = dynamic_cast< Reactive* >((*itrV)))
                    {
                        reactElement->reactiveScale( zoomFactor );
                    }
                    itrV++;
                }
                itrZ++;
            }
        }

        private:
        sf::Clock clock;
        sf::Time time;
        uint64_t fps = 0;
        vector< sf::Drawable* > sfPrimitives;
        map< uint8_t, vector< Visible* > > visible;
        map< uint8_t, vector< Visible* > > ui;

    };
}
#endif