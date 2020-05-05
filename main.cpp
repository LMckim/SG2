#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Resource.hpp>
#include <src/Manager/Object.hpp>
#include <src/Manager/Event.hpp>

#include <src/Generator/Layout.hpp>
// TESTS (remove before PROD)
int main()
{
    std::srand(std::time(nullptr));
    
    SG::Manager::Screen screenM;
    SG::Manager::Resource resourceM;
    SG::Manager::Object objectM( &screenM );
    SG::Manager::Event eventM( screenM.window );

    SG::Generator::Layout lay( &objectM );
    sf::Image test, floor;
    test.loadFromFile("assets\\Layouts\\Layout_test.png");
    floor.loadFromFile("assets\\graphics\\floors.png");
    sf::Texture* floorT = new sf::Texture();
    sf::Texture* doorT = new sf::Texture();
    sf::Texture* wallT = new sf::Texture();
    
    std::cout << floor.getSize().x << " " << floor.getSize().y << std::endl;
    floorT->loadFromImage( floor, sf::IntRect(0,32,32,32) );
    doorT->loadFromImage(  floor, sf::IntRect(0,0,32,32) );
    wallT->loadFromImage(  floor, sf::IntRect(32,32,32,32) );
    lay.addAsset(SG::Generator::Layout::FLOOR, floorT);
    lay.addAsset(SG::Generator::Layout::DOOR, doorT);
    lay.addAsset(SG::Generator::Layout::WALL, wallT);
    lay.generateLayout(&test);


    while(screenM.window->isOpen())
    {
        eventM.handleEvents();
        objectM.update();
        screenM.render();
    }
}