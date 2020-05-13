#ifndef SG_WINDOW_BASE
#define SG_WINDOW_BASE

#include <vector>
#include <string>

#include <src/Primitive/Visible/Reactive.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Active.hpp>

#include <src/Window/Button.hpp>
#include <src/Tool/TextureSheet.hpp>

namespace SG::Manager{
    class Object;
}

namespace SG::Window
{
    using std::vector;
    using std::string;
    using SG::Manager::Object;
    using SG::Primitive::Z_LAYERS;
    using SG::Primitive::Reactive;
    using SG::Primitive::Variable;
    using SG::Primitive::Active;
    using SG::Tool::TextureSheet;
    using SG::Window::Button;

    class BaseWindow :
        virtual public Reactive,
        virtual public Variable,
        virtual public Active
    {
        public:
        BaseWindow(TextureSheet* windowSheet, Active* reference, sf::Font* font, string title, uint8_t width, uint8_t height, bool buildHeader = true ) :
             sheet{ windowSheet }, reference{ reference }, font{ font }, title{ title }, width{ width }, height{ height }
        {
            this->zLevel = Z_LAYERS::WINDOW_BASE;
            this->draggable = true;
        } 

        void buildWindow()
        {
            sf::RenderTexture texture;
            // create the window and an extra section for the header/ title
            texture.create( this->width * SECTION_SIZE + SECTION_SIZE, this->height * SECTION_SIZE + SECTION_SIZE );
            texture.clear( sf::Color::Transparent );

            vector< sf::Sprite > sections;
            // ************ HEADER **************
            // build the header
            for(size_t headerX = 1; headerX < this->width; headerX++)
            {
                sf::Sprite sprite;
                sf::Texture* section;
                // left header
                if(headerX == 1)
                {
                    section = this->sheet->getTexture(2,1);
                    // right header
                }else if(headerX == (uint16_t)( this->width - 1)){
                    section = this->sheet->getTexture(2,2);
                    // middle header
                }else{
                    section = this->sheet->getTexture(2,3);
                }

                sprite.setTexture( *section );
                sprite.setPosition( headerX * SECTION_SIZE, 0 );
                sections.push_back( sprite );
            }
            // build the header click box
            sf::RectangleShape header;
            header.setFillColor( sf::Color::Transparent );
            header.setOutlineColor( CLICK_BOX_COLOR );
            header.setOutlineThickness( CLICK_BOX_OUTLINE_THICKNESS );
            header.setSize(
                sf::Vector2f(
                    (this->width * SECTION_SIZE * SCALE_ADJ) - (SECTION_SIZE * 2 * SCALE_ADJ),
                    SECTION_SIZE * SCALE_ADJ
                )
            );


            // ************ BODY **************
            // build the main window section
            for(size_t y = 1; y < this->height + SECTION_SIZE; y++)
            {
                for(size_t x = 0; x < this->width + SECTION_SIZE; x++)
                {   
                    sf::Sprite sprite;
                    sf::Texture* section;
                    // top-left corner
                    if(y == 1 && x == 0)
                    {
                        section = this->sheet->getTexture(0,0);
                        // top-right corner
                    }else if(y == 1 && (x == this->width) ){ 
                        section = this->sheet->getTexture(0,1);
                        // bottom left corner
                    }else if( (y == this->height) && x == 0){
                        section = this->sheet->getTexture(0,3);
                        // bottom-right corner
                    }else if( (y == this->height) && (x == this->width) ){
                        section = this->sheet->getTexture(0,2);
                        // top edge
                    }else if( y == 1){
                        section = this->sheet->getTexture(1,3);
                        // left edge
                    }else if( x == 0){
                        section = this->sheet->getTexture(1,0);
                        // bottom edge
                    }else if( y == (this->height) ){
                        section = this->sheet->getTexture(1,2);
                        // right edge
                    }else if( x == (this->width) ){
                        section = this->sheet->getTexture(1,1);
                    }else{
                        section = this->sheet->getTexture(2,0);
                    }
                    sprite.setTexture( *section );
                    sprite.setPosition( x * SECTION_SIZE, y * SECTION_SIZE );
                    sections.push_back( sprite );
                }
            }

            for( auto &itr : sections )
            {
                texture.draw( itr );
            }

            // ************ TEXT **************
            // add in our window title
            sf::Text titleText;
            titleText.setFont( *this->font );
            titleText.setScale(0.5,0.5);
            titleText.setOutlineColor( sf::Color::Black );
            titleText.setFillColor( sf::Color::White );
            titleText.setString( this->title );
            titleText.setPosition( 
                (this->width * SECTION_SIZE) / 2 - (SECTION_SIZE * 1.5),
                0
            );
            texture.draw( titleText );

            // render out the texture
            texture.display();
            this->texture = texture.getTexture();
            this->sprite.setTexture( this->texture );
            this->sprite.setScale( SCALE_ADJ, SCALE_ADJ);

            // position our click boxes
            // TODO: better click box positioning
            header.setPosition(
                sf::Vector2f(
                    this->sprite.getPosition().x + (SECTION_SIZE * 2 * 2) + (SECTION_SIZE / 2),
                    this->sprite.getPosition().y
                )
            );
            this->clickBoxes.push_back( header );

            this->originalHeight = this->sprite.getGlobalBounds().height;
            this->originalWidth = this->sprite.getGlobalBounds().width;
        }

        void addButton(sf::Texture *icon, uint16_t sectionsX = 2, uint16_t sectionsY = 2, string text = "")
        {
            sf::RenderTexture newTex;
            newTex.create( 64, 64 );
            newTex.clear( sf::Color::Transparent );
            
            vector< sf::Sprite > sections;
            for(size_t y = 0; y < sectionsY; y++)
            {
                for(size_t x = 0; x < sectionsX; x++)
                {
                    sf::Sprite section;
                    sf::Texture* sectionTex;
                    if( y == 0 && x == 0 )
                    {
                        sectionTex = this->sheet->getTexture(3,0);
                    }else if( y == 0 && x == (uint16_t)(sectionsX - 1) ){
                        sectionTex = this->sheet->getTexture(3,1);
                    }else if( y == (uint16_t)(sectionsY - 1) && x == 0 ){
                        sectionTex = this->sheet->getTexture(3,3);
                    }else if( y == (uint16_t)(sectionsY - 1) && x == (uint16_t)(sectionsX - 1) ){
                        sectionTex = this->sheet->getTexture(3,2);
                    }
                    section.setTexture( *sectionTex );
                    section.setPosition(
                        x * SECTION_SIZE,
                        y * SECTION_SIZE
                    );
                    sections.push_back( section );
                }
            }

            for( auto &itr : sections )
            {
                newTex.draw( itr );
            }

            sf::Sprite icon_S;
            icon_S.setTexture( *icon );
            icon_S.setOrigin(
                sf::Vector2f(
                    icon->getSize().x / 2,
                    icon->getSize().y / 2
                )
            );
            icon_S.setPosition(
                sf::Vector2f(
                    icon_S.getTexture()->getSize().x / 2,
                    icon_S.getTexture()->getSize().y / 2
                )
            );
            newTex.draw( icon_S );
            newTex.display();

            sf::Texture* buttonTex = new sf::Texture();
            *buttonTex = newTex.getTexture();
            Button newBtn(buttonTex, this->font, text);
            this->buttons.push_back( newBtn );

        }

        protected:
        const sf::Color CLICK_BOX_COLOR = sf::Color::Cyan;
        const float CLICK_BOX_OUTLINE_THICKNESS = 0.75;
        const uint8_t SECTION_SIZE = 16;
        float SCALE_ADJ = 2;
        TextureSheet* sheet;

        bool buildHeader = false;
        bool showBoxes = true;
        Active* reference;
        sf::Font* font;
        string title;
        uint8_t width;
        uint8_t height;
        sf::Texture texture;

        // used to reference reactive window buttons
        uint16_t originalWidth;
        uint16_t originalHeight;

        // click boxes are divided into
        //  0 == header (for dragging)
        //  1 == body for w/e
        //  3 == exit button
        vector< sf::RectangleShape > clickBoxes;
        vector< Button > buttons;
        virtual void update() {};
        virtual void handleInput() {};
        virtual void reactiveScale( float zoomFactor )
        {
            // TODO: implement better scaling
            this->sprite.setScale(
                this->sprite.getScale().x * zoomFactor,
                this->sprite.getScale().y * zoomFactor
            );
            // clickbox scaling
            for( auto &itr : this->clickBoxes )
            {
                itr.setScale(
                    itr.getScale().x * zoomFactor,
                    itr.getScale().y * zoomFactor
                );
            }
            // button scaling
            for( auto &itr : this->buttons )
            {
                itr.sprite.setScale(
                    itr.sprite.getScale().x * zoomFactor,
                    itr.sprite.getScale().y * zoomFactor
                );
            }
            this->positionButtons();
        }

        virtual void drag( sf::Vector2f mPos )
        {
            // TODO: better drag positioning
            // TODO: better header clicked sensing
            if(this->clickBoxes[0].getGlobalBounds().contains( mPos ) || this->beingDragged)
            {
                this->beingDragged = true;
                this->clickBoxes[0].setPosition(
                    // position the box at the mouse position minus half its width
                    mPos.x - this->clickBoxes[0].getGlobalBounds().width / 2,
                    mPos.y - this->clickBoxes[0].getGlobalBounds().height / 2
                );
                this->sprite.setPosition( 
                    sf::Vector2f(
                        // get the position of the header, minus a section ( header should always be in the middle ), multiply by the scaled ratio
                        this->clickBoxes[0].getPosition().x - ((SECTION_SIZE * SCALE_ADJ) * (this->sprite.getGlobalBounds().width / this->originalWidth)),
                        this->clickBoxes[0].getPosition().y
                    ) 
                );
            }
            this->positionButtons();
        }
        virtual void draw(sf::RenderTarget& target)
        {
            target.draw( this->sprite );
            for( auto &itr : this->buttons )
            {
                itr.draw( target );
            }
            if(this->showBoxes)
            {
                for(auto &itr : this->clickBoxes)
                {
                    target.draw( itr );
                }
            }
        }
        void positionButtons()
        {
            // TODO: setup grid placement within the window
            auto btns = this->buttons.begin();
            // get ratios
            float ratioX = this->sprite.getGlobalBounds().height / this->originalHeight;
            float ratioY = this->sprite.getGlobalBounds().width / this->originalWidth;
            
            int refX = this->sprite.getPosition().x;
            int refY = this->sprite.getPosition().y;

            while( btns != this->buttons.end() )
            {
                (*btns).sprite.setPosition(
                    sf::Vector2f(
                        refX + ( 100  * ratioX ),
                        refY + ( 100  * ratioY )
                    )
                );
                btns++;
            }
        }
    };
}


#endif