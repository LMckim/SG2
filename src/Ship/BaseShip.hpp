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
        BaseShip( Object* _objectM, sf::Texture* _shipSprite, Layout _layout ) : objectM{ _objectM }, layout{ _layout }
        {
            this->zLevel = Z_LAYERS::SHIP_GRAPHIC;
            this->sprite.setTexture( *_shipSprite );
            this->sprite.setOrigin(
                sf::Vector2f(
                    _shipSprite->getSize().x / 2,
                    _shipSprite->getSize().y / 2
                )
            );
            this->sprite.setPosition(
                this->layout.getCenterNode()->getPosition()->x,
                this->layout.getCenterNode()->getPosition()->y
            );
            this->buildShadowOverlay();
        }
        void generateCrew(uint16_t count = 1)
        {
            // TESTING SHIT
            sf::Image cImg;
            cImg.loadFromFile("assets\\graphics\\personnel_luke_sheet.png");
            sf::Texture* cTex = new sf::Texture();
            cTex->loadFromImage(cImg, sf::IntRect(0,0,16,16));

            // this may stay
            for(size_t crewItr = 0; crewItr < count; crewItr++)
            {
                Node* spawn = this->layout.getRandomFloorNode();
                if( spawn->isOccupied() ) continue;
                BaseCrew* guy = new BaseCrew( cTex, spawn );
                this->objectM->addVariable( guy, false );
                this->crew.push_back( guy );
            }
        }
        bool alive = true;
        string name = "CARGO_01";
        virtual ~BaseShip() {}
        
        protected:
        static const uint8_t DARKNESS = 200;
        Object* objectM;
        Layout layout;

        sf::Texture shadowTextureBase;
        sf::Texture shadowTextureLit;
        sf::Sprite shadow;
        vector< BaseCrew* > crew;

        virtual void buildShadowOverlay()
        {
            // final texture
            sf::RenderTexture shadow;
            shadow.create(
                this->sprite.getTexture()->getSize().x, 
                this->sprite.getTexture()->getSize().y 
            );
            shadow.clear( sf::Color::Transparent );
            // shadow node
            sf::RenderTexture shadowNode;
            shadowNode.create(16,16);
            // ADJUST DARKNESS HERE
            shadowNode.clear( sf::Color(0, 0, 0, DARKNESS) );
            shadowNode.display();

            sf::Sprite shadowSprite;
            shadowSprite.setTexture( shadowNode.getTexture() );

            // draw the shadow boxes
            for( auto &itr : this->layout.visible )
            {
                shadowSprite.setPosition( itr->getPosition()->x, itr->getPosition()->y );
                shadow.draw( shadowSprite );
            }
            shadow.display();

            this->shadowTextureBase = shadow.getTexture(); 
            this->shadow.setTexture( this->shadowTextureBase );
            this->shadow.setOrigin( this->sprite.getOrigin() );
            this->shadow.setPosition(
                this->sprite.getPosition()
            );

        }
        virtual void updateShadowOverlay()
        {
            // basically copies the unlit shadow
            sf::Sprite temp;
            sf::RenderTexture updatedShadow;
            updatedShadow.create( 
                this->shadowTextureBase.getSize().x,
                this->shadowTextureBase.getSize().y
            );
            updatedShadow.clear( sf::Color::Transparent );

            // create our light
            sf::RenderTexture lightTex;
            lightTex.create(48,48);
            // lightTex.clear( sf::Color(255,252,102,50) );
            lightTex.clear( sf::Color::Transparent );
            lightTex.display();
            
            // draw the darkness
            sf::Texture tempT = lightTex.getTexture();
            temp.setTexture( this->shadowTextureBase );
            updatedShadow.draw( temp );
            // draw the light around each crew
            temp = sf::Sprite();
            temp.setTexture( tempT );
            temp.setOrigin(
                tempT.getSize().x / 2,
                tempT.getSize().y / 2
            );
            for( auto &itr : this->crew )
            {
                temp.setPosition( 
                    itr->sprite.getPosition().x + (itr->sprite.getTexture()->getSize().x / 2), 
                    itr->sprite.getPosition().y
                );
                updatedShadow.draw( temp, sf::BlendNone );
            }
            updatedShadow.display();

            this->shadowTextureLit = updatedShadow.getTexture();
            this->shadow.setTexture( this->shadowTextureLit );

        }
        virtual void update()
        {

        }
        virtual void draw(sf::RenderTarget& target)
        {
            // draw the ship
            target.draw( this->sprite );
            // draw the layout (floor, walls, etc)
            this->layout.draw( target );
            // draw the crew
            for( auto &itr : this->crew ) itr->draw( target );
            // draw the shadow overlay
            this->updateShadowOverlay();
            target.draw( this->shadow );
        }
        private:
    };
}


#endif