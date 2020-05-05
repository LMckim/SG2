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
    sf::Image test;
    test.loadFromFile("assets\\Layouts\\Layout_test.png");
    lay.generateLayout(&test);
}