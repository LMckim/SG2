#ifndef SG_MANAGER_RESOURCE
#define SG_MANAGER_RESOURCE

#include <map>

#include <SFML/Graphics.hpp>


using std::map;
using std::string;
namespace SG::Manager
{
    class Resource
    {
        public:
        sf::Image* getImage(string id)
        {
            if(this->images.find( id ) == this->images.end() )
            {
                throw "Could not locate specified image, ID:" + id;
            }else return &this->images[ id ];
        }
        sf::Texture* getTexture(string id)
        {
            if(this->textures.find( id ) == this->textures.end() )
            {
                throw "Could not locate specified texture, ID:" + id;
            }else return &this->textures[ id ];
        }
        private:
        map< string, sf::Image > images;
        map< string, sf::Texture > textures;
    };
}


#endif