#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Resource.hpp>
#include <src/Manager/Object.hpp>
#include <src/Manager/Event.hpp>
#include <src/Tool/TextureSheet.hpp>

// temp for testing
#include <src/Generator/Gen_Layout.hpp>
#include <src/Ship/BaseShip.hpp>
#include <src/Ship/Layout.hpp>
#include <src/Window/BaseWindow.hpp>

#include <src/Object/BaseObject.hpp>
#include <src/Window/SpawnerWindow.hpp>

// TESTS (remove before PROD)
int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    SG::Manager::Screen screenM;
    SG::Manager::Resource resourceM;
    SG::Manager::Object objectM( &screenM );
    SG::Manager::Event eventM( &screenM, &objectM );

    sf::Font font;
    font.loadFromFile("assets\\font\\munro.ttf");


    // EVERYTHING UNDER HERE IS TEMPORARY, BASED ON IN-GAME
    // LAYOUT AND SHIP TEST
    SG::Tool::TextureSheet* floorSections = new SG::Tool::TextureSheet("assets\\graphics\\sheets\\floor_sheet.png");
    SG::Tool::TextureSheet* interiorWallSections = new SG::Tool::TextureSheet("assets\\graphics\\sheets\\interior_wall.png");
    SG::Tool::TextureSheet* doorSections = new SG::Tool::TextureSheet("assets\\graphics\\sheets\\doors.png");

    SG::Generator::Gen_Layout lay( &objectM );
    sf::Image test;
    sf::Texture* ship = new sf::Texture();
    ship->loadFromFile("assets\\graphics\\ships\\CARGO_01.png");
    test.loadFromFile("assets\\layouts\\CARGO_01.png");

     
    lay.addAsset(SG::Generator::Gen_Layout::TILE::FLOOR, floorSections );
    lay.addAsset(SG::Generator::Gen_Layout::TILE::DOOR, doorSections );
    lay.addAsset(SG::Generator::Gen_Layout::TILE::WALL, interiorWallSections );

    SG::Ship::Layout layout = lay.generateLayout( &test );
    objectM.registerLayout( &layout );
    SG::Ship::BaseShip shipBase( &objectM, ship, layout);
    objectM.addVisible( &shipBase );
    shipBase.generateCrew(5);

    // WINDOW TEST
    sf::Image windowImg;
    SG::Tool::TextureSheet* windowTS = new SG::Tool::TextureSheet("assets\\graphics\\sheets\\window_1.png");
    SG::Window::BaseWindow window( windowTS, &font,"FUCK YEAH", 8, 6);
    window.buildWindow();

    // BUTTON TEST
    sf::Texture* icon = new sf::Texture();
    icon->loadFromFile("assets\\graphics\\objects\\build_console_abl.png");
    window.addIconButton( icon );
    window.addTextButton( "FUCK", 3 );
    objectM.addVariable( &window );

    // SPAWNER WINDOW TEST
    SG::Tool::TextureSheet* objSheet = new SG::Tool::TextureSheet("assets\\graphics\\sheets\\build_console_abl.png",32,32);
    SG::Object::BaseObject* obj = new SG::Object::BaseObject(objSheet);
    SG::Window::SpawnerWindow s_window(&objectM, windowTS, &font,"SPAWN", 6,6);
    s_window.addObjectSpawnButton( objSheet->getTexture(0,0), obj);
    s_window.buildWindow();
    objectM.addVariable(&s_window);

    // HERES THE HEART
    while(screenM.window->isOpen())
    {
        eventM.handleEvents();
        objectM.update();
        screenM.render();
    }
}