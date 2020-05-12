#ifndef SG_WINDOW_BASE
#define SG_WINDOW_BASE

#include <vector>
#include <string>

#include <src/Primitive/Visible.hpp>
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
    using SG::Primitive::Visible;
    using SG::Primitive::Variable;
    using SG::Primitive::Active;
    using SG::Tool::TextureSheet;

    class BaseWindow :
        virtual public Visible,
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
            header.setPosition(
                sf::Vector2f(
                    this->sprite.getPosition().x + (SECTION_SIZE * 2 * 2) + (SECTION_SIZE / 2),
                    this->sprite.getPosition().y
                )
            );
            this->clickBoxes.push_back( header );
        }

        protected:
        const sf::Color CLICK_BOX_COLOR = sf::Color::Cyan;
        const float CLICK_BOX_OUTLINE_THICKNESS = 0.75;
        const uint8_t SECTION_SIZE = 16;
        const int SCALE_ADJ = 3;
        TextureSheet* sheet;

        bool buildHeader = false;
        bool showBoxes = true;
        Active* reference;
        sf::Font* font;
        string title;
        uint8_t width;
        uint8_t height;
        sf::Texture texture;

        // click boxes are divided into
        //  0 == header (for dragging)
        //  1 == body for w/e
        //  3 == exit button
        vector< sf::RectangleShape > clickBoxes;
        vector< Button > buttons;
        virtual void update() {};
        virtual void handleInput() {};
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
    };
}


#endif