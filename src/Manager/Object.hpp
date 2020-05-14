#ifndef SG_MANAGER_OBJECT
#define SG_MANAGER_OBJECT

#include <vector>

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Active.hpp>
#include <src/Primitive/Active/Draggable.hpp>
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
    using SG::Primitive::Draggable;
    using SG::Ship::Layout;
    using SG::Primitive::Node;

    class Object
    {
        public:
        bool dragging = false;

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
        void addUI( Visible* visible )
        {
            this->screenM->addUI( visible );
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

        void leftClick(sf::Vector2f mPos)
        {
            // clear our selection
            this->selected.clear();
            // check if any objects are within the clicked area
            for(auto &itr : this->actives)
            {
                if( itr->sprite.getGlobalBounds().contains( mPos ) )
                {
                    if(Draggable* draggable = dynamic_cast< Draggable* >( itr ))
                    {
                        if(draggable->drag( mPos ))
                        {
                            this->dragging = true;
                            this->DraggedObj = draggable;
                            this->DraggedObj->forceDrag( mPos );
                        }else{
                            itr->select( mPos );
                            this->selected.push_back( itr );
                        }
                        break;
                    }else{
                        itr->select( mPos );
                        this->selected.push_back( itr );
                        break;
                    }
                }else{
                    // everything NOT within the clicked area should be de-selected
                    itr->selected = false;
                }
            }
        }
        void leftDrag(sf::Vector2f mPos)
        {
            this->DraggedObj->forceDrag( mPos );
        }
        void clearDrag()
        {
            std::cout  << "clear\n";
            this->dragging = false;
            this->DraggedObj = nullptr;
        }
        void rightClicked(sf::Vector2f mPos)
        {
            this->layout->clearPathing();
            Node* dest = this->layout->findClosestNode( mPos );
            if(dest == nullptr) throw "Could not find Node!";
            
            for(auto &itr : this->selected)
            {
                itr->rightClick( dest );
            }
        }

        void processSelectionBox( sf::RectangleShape* selectionBox)
        {
            // clear our selected
            for(size_t i=0; i < this->selected.size(); i++)
            {
                this->selected[i]->selected = false;   
            }
            this->selected.clear();
            // select new within the selection box
            for( auto &itr : this->actives )
            {
                if( selectionBox->getGlobalBounds().contains( itr->sprite.getPosition() ) )
                {
                    itr->selected = true;
                    this->selected.push_back( itr );
                }
            }
            // cleanup to eliminate any non-group selectable 
            auto itr = this->selected.begin();
            while( itr != this->selected.end())
            {
                if((*itr)->groupselect == false)
                {
                    itr = this->selected.erase( itr );
                }else itr++;
            }
        }

        private:
        bool paused = false;

        const sf::Color SELECTION_BOX_COLOR = sf::Color::Green;
        const float SELECTION_BOX_THICKNESS = 1.f;

        Screen* screenM;
        Layout* layout;
        vector< Variable* > variables;
        vector< Active* > actives;
        vector< Active* > selected;
        Draggable* DraggedObj;
        
    };
}


#endif