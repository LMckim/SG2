#ifndef SG_MANAGER_OBJECT
#define SG_MANAGER_OBJECT

#include <vector>

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Active.hpp>
#include <src/Ship/Layout.hpp>
#include <src/Primitive/Node.hpp>

namespace SG::Manager
{
    using std::vector;
    using std::is_convertible;
    using SG::Manager::Screen;
    using SG::Primitive::Visible;
    using SG::Primitive::Variable;
    using SG::Primitive::Active;
    using SG::Ship::Layout;
    using SG::Primitive::Node;

    class Object
    {
        public:
        Object(Screen* screenM) : screenM{ screenM } {}

        void update()
        {
            if(!this->paused)
            {
                for( auto itr : this->variables )
                {
                    itr->update();
                }
                this->removeVariables();
            }
        }

        void togglePause()
        {
            this->paused == true ? this->paused = false : this->paused = true;
        }

        void addVisible(Visible* visible)
        {
            this->screenM->addVisible( visible );
        }

        void addVariable(Variable* variable)
        {
            this->variables.push_back( variable );
            if(Visible* visible = dynamic_cast<Visible*>( variable ))
            {
                this->screenM->addVisible( visible );
            }
            if(Active* active = dynamic_cast<Active*>( variable ))
            {
                this->actives.push_back( active );
            }
        }

        void registerLayout(Layout* layout)
        {
            this->layout = layout;
        }

        void removeVariables()
        {
            auto itr = this->variables.begin();
            while( itr != this->variables.end() )
            {
                if((*itr)->destroy)
                {
                    Variable* temp = (*itr);
                    itr = this->variables.erase( itr );

                    if(Visible* visible = dynamic_cast< Visible* >( temp ))
                    {
                        this->screenM->removeVisible( visible );
                    }
                    delete temp;
                }else itr++;
            }
        }

        void checkClicked(sf::Vector2f mPos)
        {
            this->quickClick(mPos);
        }

        void quickClick(sf::Vector2f mPos)
        {
            // TODO: Problems with selection WILL occur HERE, fix when ready
            for(auto &itr : this->actives)
            {
                if( itr->sprite.getGlobalBounds().contains( mPos ) )
                {
                    itr->select();
                    this->selected.push_back( itr );
                }else{
                    itr->selected = false;
                    this->selected.clear();
                }
            }
        }

        void rightClicked(sf::Vector2f mPos)
        {
            this->layout->clearPathing();
            Node* dest = this->layout->findClosestNode( mPos );
            if(dest == nullptr) throw "Could not find Node!";
            
            for(auto &itr : this->selected)
            {
                std::cout << "r clicked\n";
                itr->rightClick( dest );
            }
        }

        void processSelectionBox( sf::RectangleShape* selectionBox)
        {
            for( auto &itr : this->actives )
            {
                if( selectionBox->getGlobalBounds().contains( itr->sprite.getPosition() ) )
                {
                    itr->selected = true;
                    this->selected.push_back( itr );
                }else{
                    itr->selected = false;
                    for(size_t i=0; i < this->selected.size(); i++)
                    {
                        if( itr == this->selected[i] )
                        {
                            this->selected.erase( this->selected.begin() + i );   
                        }
                        break;
                    }
                }
            }
        }

        private:
        bool paused = false;

        bool clickedThisCycle = false;
        bool stillClicked = false;
        const sf::Color SELECTION_BOX_COLOR = sf::Color::Green;
        const float SELECTION_BOX_THICKNESS = 1.f;

        Screen* screenM;
        Layout* layout;
        vector< Variable* > variables;
        vector< Active* > actives;
        vector< Active* > selected;
        
    };
}


#endif