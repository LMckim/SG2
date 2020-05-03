#ifndef SG_MANAGER_OBJECT
#define SG_MANAGER_OBJECT

#include <vector>

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Primitive/Visible.hpp>
#include <src/Primitive/Variable.hpp>

using std::vector;
using std::is_convertible;
using SG::Manager::Screen;
using SG::Primitive::Visible;
using SG::Primitive::Variable;

namespace SG::Manager
{
    class Object
    {
        public:
        Object(Screen* screenM) : screenM{ screenM } {}
        void update()
        {
            for( auto itr : this->variables )
            {
                itr->update();
            }
            this->removeVariables();
        }
        void addVariable(Variable* variable)
        {
            this->variables.push_back( variable );
            if(Visible* visible = dynamic_cast<Visible*>( variable ))
            {
                this->screenM->addVisible( visible );
            }
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
        private:
        Screen* screenM;
        vector< Variable* > variables; 
        
    };
}


#endif