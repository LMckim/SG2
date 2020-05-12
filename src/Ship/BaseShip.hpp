#ifndef SG_SHIP_BASE
#define SG_SHIP_BASE

#include <string>
#include <vector>
#include <src/Manager/Object.hpp>
#include <src/Crew/BaseCrew.hpp>
#include <src/Ship/Layout.hpp>
#include <src/Primitive/Node.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Visible.hpp>

namespace SG::Ship
{
    using std::string;
    using std::vector;
    using SG::Manager::Object;
    using SG::Crew::Base;
    using SG::Ship::Layout;
    using SG::Primitive::Node;
    using SG::Primitive::Z_LAYERS;
    using SG::Primitive::Visible;
    using SG::Primitive::Variable;

    using BaseCrew = SG::Crew::Base;

    class BaseShip  :   
        virtual public Visible,
        virtual public Variable
    {
        public:
        BaseShip( Object* objectM, sf::Texture* shipSprite, Layout layout ) : objectM{ objectM }, layout{ layout }
        {
            this->zLevel = Z_LAYERS::SHIP_GRAPHIC;
            this->sprite.setTexture( *shipSprite );
            this->sprite.setOrigin(
                sf::Vector2f(
                    shipSprite->getSize().x / 2,
                    shipSprite->getSize().y / 2
                )
            );
            this->sprite.setPosition(
                this->layout.getCenterNode()->getPosition()->x,
                this->layout.getCenterNode()->getPosition()->y
            );
        }
        void generateCrew(uint16_t count = 1)
        {
            // TESTING SHIT
            sf::Image cImg;
            cImg.loadFromFile("assets\\graphics\\personnel_luke_sheet.png");
            sf::Texture* cTex = new sf::Texture();
            cTex->loadFromImage(cImg, sf::IntRect(0,0,16,16));

            // this may stay
            for(size_t crew = 0; crew < count; crew++)
            {
                Node* spawn = this->layout.getRandomFloorNode();
                if( spawn->isOccupied() ) continue;
                BaseCrew* guy = new BaseCrew( cTex, spawn );
                this->objectM->addVariable( guy );
                this->crew.push_back( guy );
            }
        }
        bool alive = true;
        string name = "TESTER";
        virtual ~BaseShip() {}
        
        protected:
        Object* objectM;
        Layout layout;
        vector< BaseCrew* > crew;

        virtual void update()
        {

        }
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->sprite );
        }
        private:
    };
}


#endif