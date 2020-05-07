#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Resource.hpp>
#include <src/Manager/Object.hpp>
#include <src/Manager/Event.hpp>

#include <src/Generator/Layout.hpp>
#include <src/Ship/BaseShip.hpp>
#include <src/Ship/Layout.hpp>
// TESTS (remove before PROD)
int main()
{
    std::srand(std::time(nullptr));
    
    SG::Manager::Screen screenM;
    SG::Manager::Resource resourceM;
    SG::Manager::Object objectM( &screenM );
    SG::Manager::Event eventM( &screenM, &objectM );

    // EVERYTHING UNDER HERE IS TEMPORARY
    SG::Generator::Layout lay( &objectM );
    sf::Image test, floor;
    test.loadFromFile("assets\\Layouts\\Layout_test.png");
    floor.loadFromFile("assets\\graphics\\floor_sheet.png");
    sf::Texture* floorT = new sf::Texture();
    sf::Texture* doorT = new sf::Texture();
    sf::Texture* wallT = new sf::Texture();
    
    floorT->loadFromImage( floor, sf::IntRect(0,16,16,16) );
    doorT->loadFromImage(  floor, sf::IntRect(16,0,16,16) );
    wallT->loadFromImage(  floor, sf::IntRect(0,0,16,16) );
    lay.addAsset(SG::Generator::Layout::FLOOR, floorT);
    lay.addAsset(SG::Generator::Layout::DOOR, doorT);
    lay.addAsset(SG::Generator::Layout::WALL, wallT);

    SG::Ship::Layout layout = lay.generateLayout( &test );
    objectM.registerLayout( &layout );
    sf::Texture* ship = new sf::Texture();
    ship->loadFromFile("assets\\graphics\\ship_cargo_full.png");
    SG::Ship::BaseShip shipBase( &objectM, ship, layout);
    objectM.addVisible( &shipBase );

    shipBase.generateCrew();
    // HERES THE HEART
    while(screenM.window->isOpen())
    {
        eventM.handleEvents();
        objectM.update();
        screenM.render();
    }
}