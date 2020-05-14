#ifndef SG_WINDOW_BUTTON_ICON
#define SG_WINDOW_BUTTON_ICON

#include <vector>
#include <string>

#include <src/Window/Button.hpp>

namespace SG::Window
{
    using std::string;
    using SG::Window::Button;

    class IconButton    :
        virtual protected Button
    {
        friend class BaseWindow;

        public:
        IconButton(sf::Texture *buttonTexture) : Button{ buttonTexture }
        {
        }
        virtual ~IconButton()
        {
        }
        protected:
    };
}


#endif