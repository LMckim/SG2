#ifndef SG_WINDOW_BASE
#define SG_WINDOW_BASE

#include <vector>
#include <string>

#include <src/Primitive/Visible/Reactive.hpp>
#include <src/Primitive/Variable.hpp>
#include <src/Primitive/Active.hpp>

#include <src/Window/Button/IconButton.hpp>
#include <src/Window/Button/TextButton.hpp>
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
        virtual ~BaseWindow()
        {
            while(!this->buttons.empty()) delete this->buttons.back(), this->buttons.pop_back();
        }
        void buildWindow()
        {
            sf::RenderTexture texture;
            // create the window and an extra section for the header/ title
            texture.create( this->width * SECTION_SIZE + SECTION_SIZE, this->height * SECTION_SIZE + SECTION_SIZE );
            texture.clear( sf::Color::Transparent );

            vector< sf::Sprite > sections;
            // ************ HEADER **************
            // build the reactive lock button
            sf::Sprite lock;
            lock.setTexture( *this->sheet->getTexture(3,1) );
            lock.setPosition(0,0);
            texture.draw( lock );
            // build the lock click box
            sf::RectangleShape lockBox;
            lockBox.setFillColor( sf::Color::Transparent );
            lockBox.setOutlineColor( CLICK_BOX_COLOR );
            lockBox.setOutlineThickness( CLICK_BOX_OUTLINE_THICKNESS );
            lockBox.setSize(
                sf::Vector2f(
                    SECTION_SIZE * SCALE_ADJ, 
                    SECTION_SIZE * SCALE_ADJ
                )
            );
            this->clickBoxes.push_back( lockBox );
            // build the header title section
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
                    // the width of the box minus 2 sections
                    ((this->width * SECTION_SIZE) * SCALE_ADJ) - (SECTION_SIZE * SCALE_ADJ),
                    SECTION_SIZE * SCALE_ADJ
                )
            );
            this->clickBoxes.push_back( header );
            // build the exit button
            sf::Sprite exit;
            exit.setTexture( *this->sheet->getTexture(3,2) );
            exit.setPosition( this->width * SECTION_SIZE, 0 );
            texture.draw( exit );
            // build the lock click box
            sf::RectangleShape exitBox;
            exitBox.setFillColor( sf::Color::Transparent );
            exitBox.setOutlineColor( CLICK_BOX_COLOR );
            exitBox.setOutlineThickness( CLICK_BOX_OUTLINE_THICKNESS );
            exitBox.setSize(
                sf::Vector2f(
                    SECTION_SIZE * SCALE_ADJ, 
                    SECTION_SIZE * SCALE_ADJ
                )
            );
            this->clickBoxes.push_back( exitBox );

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
            sf::RectangleShape contentBox;
            contentBox.setFillColor( sf::Color::Transparent );
            contentBox.setOutlineColor( CLICK_BOX_COLOR );
            contentBox.setOutlineThickness( CLICK_BOX_OUTLINE_THICKNESS );
            contentBox.setSize(
                sf::Vector2f(
                    (this->width + 1) * (SECTION_SIZE * SCALE_ADJ), // why +1? :S 
                    this->height * (SECTION_SIZE * SCALE_ADJ) 
                )
            );
            this->clickBoxes.push_back( contentBox );
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

            this->originalHeight = this->sprite.getGlobalBounds().height;
            this->originalWidth = this->sprite.getGlobalBounds().width;
            this->positionButtons();
            this->positionClickBoxes();
        }

        void addIconButton(sf::Texture *icon, uint16_t sectionsX = 2, uint16_t sectionsY = 2)
        {
            sf::RenderTexture newTex;
            this->buildButtonBackground(newTex, sectionsX, sectionsY );
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
            // TODO: keep an eye out here for a leak
            sf::Texture* buttonTex = new sf::Texture();
            *buttonTex = newTex.getTexture();
            Button* newBtn = new IconButton(buttonTex);
            this->buttons.push_back( newBtn );
            this->positionButtons();

        }
        
        void addTextButton(string text, uint16_t sectionsX = 2, uint16_t sectionsY = 2)
        {
            sf::RenderTexture newTex;
            this->buildButtonBackground(newTex, sectionsX, sectionsY );
            newTex.display();
            // TODO: keep an eye out here for a leak
            sf::Texture* buttonTex = new sf::Texture();
            *buttonTex = newTex.getTexture();
            Button* newBtn = new TextButton(buttonTex, this->font, text);
            this->buttons.push_back( newBtn );
            this->positionButtons();

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
        //  0 == reactive lock
        //  1 == header (for dragging)
        //  2 == exit button
        //  3 == body click area
        vector< sf::RectangleShape > clickBoxes;
        vector< Button* > buttons;
        virtual void update() 
        {
            // if(this->beingDragged == true) this->beingDragged = false;
        };
        virtual void handleInput() {};
        virtual void reactiveScale( float zoomFactor )
        {
            if(this->currentlyReactive)
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
                    itr->scale( zoomFactor );
                }
                // hacky fo sho
                this->positionClickBoxes();
                this->positionButtons();
            }
        }

        virtual void drag( sf::Vector2f mPos )
        {
            // clicked on the exit button
            if(this->clickBoxes[2].getGlobalBounds().contains( mPos ))
            {
                this->destroy = true;
            
            // reactive lock toggle
            }else if(this->clickBoxes[0].getGlobalBounds().contains( mPos )){ 
                this->currentlyReactive ? this->currentlyReactive = false : this->currentlyReactive = true;
                this->swapLockIcon();
            // content box clicked
            }else if(this->clickBoxes[3].getGlobalBounds().contains( mPos )){
                this->checkButtonClicked( mPos );
            }else if(this->clickBoxes[1].getGlobalBounds().contains( mPos ) || this->beingDragged)
            {
                // TODO: better header clicked sensing, AKA fix extra clicking
                // check the title click box
                this->beingDragged = true;
                this->positionClickBoxes( mPos );
                this->sprite.setPosition( 
                    sf::Vector2f(
                        // get the position of the header, minus a section ( header should always be in the middle ), multiply by the scaled ratio
                        this->clickBoxes[0].getPosition().x,
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
                itr->draw( target );
            }
            if(this->showBoxes)
            {
                for(auto &itr : this->clickBoxes)
                {
                    target.draw( itr );
                }
            }
        }
        // mainly for re-scaling
        void positionClickBoxes()
        {

            // lock
            this->clickBoxes[0].setPosition(
                this->sprite.getPosition().x,
                this->sprite.getPosition().y
            );
            // title
            this->clickBoxes[1].setPosition(
                this->clickBoxes[0].getGlobalBounds().left + this->clickBoxes[0].getGlobalBounds().width,
                this->clickBoxes[0].getGlobalBounds().top
            );
            // exit
            this->clickBoxes[2].setPosition(
                this->clickBoxes[1].getGlobalBounds().left + this->clickBoxes[1].getGlobalBounds().width,
                this->clickBoxes[1].getGlobalBounds().top
            );
            // body
            this->clickBoxes[3].setPosition(
                this->clickBoxes[0].getGlobalBounds().left,
                this->clickBoxes[0].getGlobalBounds().top + this->clickBoxes[0].getGlobalBounds().height
            );
        }
        // for drag positioning
        void positionClickBoxes(sf::Vector2f mPos)
        {
            // header
            this->clickBoxes[1].setPosition(
                // position the box at the mouse position minus half its width
                mPos.x - this->clickBoxes[1].getGlobalBounds().width / 2,
                mPos.y - this->clickBoxes[1].getGlobalBounds().height / 2
            );
            // lock
            this->clickBoxes[0].setPosition(
                this->clickBoxes[1].getGlobalBounds().left - this->clickBoxes[0].getGlobalBounds().width,
                this->clickBoxes[1].getGlobalBounds().top
            );
            // exit
            this->clickBoxes[2].setPosition(
                this->clickBoxes[1].getGlobalBounds().left + this->clickBoxes[1].getGlobalBounds().width,
                this->clickBoxes[1].getGlobalBounds().top
            );
            // body
            this->clickBoxes[3].setPosition(
                this->clickBoxes[0].getGlobalBounds().left,
                this->clickBoxes[0].getGlobalBounds().top + this->clickBoxes[0].getGlobalBounds().height
            );
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

            int offsetX = SECTION_SIZE * 2;
            int offsetY = SECTION_SIZE * 4;

            while( btns != this->buttons.end() )
            {
                sf::Vector2f pos = sf::Vector2f(                        
                    refX + (ratioX * offsetX),
                    refY + (ratioY * offsetY)
                );
                (*btns)->setPosition(pos);
                // TODO: more corrections of offsets to deal with window boundries
                offsetX += (*btns)->sprite.getTexture()->getSize().x + SECTION_SIZE;

                btns++;
            }
        }
        void buildButtonBackground(sf::RenderTexture& newTex, uint16_t sectionsX = 2, uint16_t sectionsY = 2)
        {
            newTex.create( SECTION_SIZE * sectionsX, SECTION_SIZE * sectionsY );
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
                        sectionTex = this->sheet->getTexture(4,0);
                    }else if( y == 0 && x == (uint16_t)(sectionsX - 1) ){
                        sectionTex = this->sheet->getTexture(4,1);
                    }else if( y == (uint16_t)(sectionsY - 1) && x == 0 ){
                        sectionTex = this->sheet->getTexture(4,3);
                    }else if( y == (uint16_t)(sectionsY - 1) && x == (uint16_t)(sectionsX - 1) ){
                        sectionTex = this->sheet->getTexture(4,2);
                    }else if(x == 0){
                        sectionTex = this->sheet->getTexture(5,0);
                    }else if(x == (uint16_t)(sectionsX - 1)){
                        sectionTex = this->sheet->getTexture(5,1);
                    }else if(y == 0){
                        sectionTex = this->sheet->getTexture(5,3);
                    }else if(y == (uint16_t)(sectionsY - 1)){
                        sectionTex = this->sheet->getTexture(5,2);
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

        }
        void checkButtonClicked(sf::Vector2f mPos)
        {
            for( auto &itr : this->buttons )
            {
                if( itr->sprite.getGlobalBounds().contains( mPos ) )
                {
                    itr->onClick();
                    break; // found the button, we can break
                }
            }
        }
        void swapLockIcon()
        {
            // TODO: fix the layering bug
            sf::Sprite newSprite;
            sf::Sprite oldSprite;
            sf::Texture* newIcon;

            if(this->currentlyReactive)
            {
                newIcon = this->sheet->getTexture(3,1);
            }else newIcon = this->sheet->getTexture(3,0);

            newSprite.setTexture( *newIcon );
            newSprite.setPosition(0,0);
            oldSprite.setTexture( this->texture );
            oldSprite.setPosition(0,0);
            
            sf::RenderTexture swap;
            swap.create( this->texture.getSize().x, this->texture.getSize().y );
            swap.clear( sf::Color::Transparent );
            swap.draw( oldSprite );
            swap.draw( newSprite );
            swap.display();

            this->texture = swap.getTexture();

        }
    };
}


#endif