#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Resource.hpp>
#include <src/Manager/Object.hpp>
#include <src/Manager/Event.hpp>
#include <src/Tool/TextureSheet.hpp>

// temp for testing
#include <src/Generator/Layout.hpp>
#include <src/Ship/BaseShip.hpp>
#include <src/Ship/Layout.hpp>
#include <src/Window/BaseWindow.hpp>
// TESTS (remove before PROD)
int main()
{
    std::srand(std::time(nullptr));
    
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

    SG::Generator::Layout lay( &objectM );
    sf::Image test;
    sf::Texture* ship = new sf::Texture();
    ship->loadFromFile("assets\\graphics\\ships\\CARGO_01.png");
    test.loadFromFile("assets\\layouts\\CARGO_01.png");

     
    lay.addAsset(SG::Generator::Layout::FLOOR, floorSections );
    lay.addAsset(SG::Generator::Layout::DOOR, doorSections );
    lay.addAsset(SG::Generator::Layout::WALL, interiorWallSections );

    SG::Ship::Layout layout = lay.generateLayout( &test );
    objectM.registerLayout( &layout );
    SG::Ship::BaseShip shipBase( &objectM, ship, layout);
    objectM.addVisible( &shipBase );
    shipBase.generateCrew(5);

    // WINDOW TEST
    sf::Image windowImg;
    SG::Tool::TextureSheet* windowTS = new SG::Tool::TextureSheet("assets\\graphics\\sheets\\window_1.png");
    SG::Window::BaseWindow window( windowTS, nullptr, &font,"FUCK YEAH", 8, 6);
    window.buildWindow();

    // BUTTON TEST
    sf::Texture* icon = new sf::Texture();
    icon->loadFromFile("assets\\graphics\\objects\\build_console_abl.png");
    window.addIconButton( icon );
    window.addTextButton( "FUCK", 3 );
    objectM.addVariable( &window );


    // HERES THE HEART
    while(screenM.window->isOpen())
    {
        eventM.handleEvents();
        objectM.update();
        screenM.render();
    }
}