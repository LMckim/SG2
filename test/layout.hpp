#include <src/Generator/Layout.hpp>
#include <src/Manager/Screen.hpp>

using SG::Generator::Layout;
using SG::Manager::Object;
using SG::Manager::Screen;
void runLayoutTest()
{
    Screen screen;
    Object obj( &screen );
    Layout lay( &obj );
    sf::Image test, floor;
    test.loadFromFile("assets\\Layouts\\Layout_test.png");
    floor.loadFromFile("assets\\graphics\\floors.png");
    sf::Texture* floorT = new sf::Texture();
    sf::Texture* doorT = new sf::Texture();
    sf::Texture* wallT = new sf::Texture();
    
    std::cout << floor.getSize().x << " " << floor.getSize().y << std::endl;
    floorT->loadFromImage( floor, sf::IntRect(0,0,32,32) );
    doorT->loadFromImage(  floor, sf::IntRect(0,0,32,32) );
    wallT->loadFromImage(  floor, sf::IntRect(0,0,32,32) );
    lay.addAsset(Layout::FLOOR, floorT);
    lay.addAsset(Layout::DOOR, doorT);
    lay.addAsset(Layout::WALL, wallT);
    lay.generateLayout(&test);
}