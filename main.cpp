#include <SFML/Graphics.hpp>

#include <src/Manager/Screen.hpp>
#include <src/Manager/Resource.hpp>
#include <src/Manager/Object.hpp>
#include <src/Manager/Event.hpp>

// TESTS (remove before PROD)
#include <test/layout.hpp>
int main()
{
    SG::Manager::Screen screenM;
    SG::Manager::Resource resourceM;
    SG::Manager::Object objectM( &screenM );
    SG::Manager::Event eventM( screenM.window );

    runLayoutTest();


    while(screenM.window->isOpen())
    {
        eventM.handleEvents();
        objectM.update();
        screenM.render();
    }
}