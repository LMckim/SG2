#ifndef SG_WINDOW_SPAWNER
#define SG_WINDOW_SPAWNER

#include <src/Manager/Object.hpp>
#include <src/Window/BaseWindow.hpp>
#include <src/Object/BaseObject.hpp>
#include <src/Object/Placement.hpp>

namespace SG::Window
{
    using std::string;
    using std::map;
    using SG::Manager::Object;
    using SG::Window::IconButton;
    using SG::Window::BaseWindow;
    using SG::Object::BaseObject;
    using SG::Object::Placement;

    class SpawnerWindow :
        public BaseWindow
    {
        public:
        SpawnerWindow(Object* _objectM, TextureSheet* _windowSheet, sf::Font* _font, string _title, uint8_t _width, uint8_t _height, bool _buildHeader = true ) :
            BaseWindow( _windowSheet, _font, _title, _width, _height, _buildHeader ), objectM{ _objectM }
        {
        }
        virtual void addObjectSpawnButton(sf::Texture* icon, BaseObject* spawnObj)
        {
            this->addIconButton(icon,
                icon->getSize().x / SECTION_SIZE,
                icon->getSize().y / SECTION_SIZE
            );
            Button* link = this->buttons.back();
            this->objectLink[ link ] = spawnObj;
        }
        protected:
        Object* objectM;
        map< Button*, BaseObject* > objectLink;

        virtual void checkButtonClicked(sf::Vector2f mPos)
        {
            for( auto &itr : this->buttons )
            {
                if( itr->sprite.getGlobalBounds().contains( mPos ) )
                {
                    this->objectM->DraggedObj = 
                        new Placement( 
                            this->objectM,
                            this->objectM->layout,
                            this->objectLink[ itr ]
                    );
                    this->objectM->DraggedObj->forceDrag( mPos );
                    this->objectM->dragging = true;
                    this->objectM->addVisible( this->objectM->DraggedObj );
                    break; // found the button, we can break
                }
            }
        }
    };

}


#endif