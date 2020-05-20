#include <SFML/Graphics.hpp>


int main() 
{
    sf::RenderWindow  window( sf::VideoMode(1200,800), "SPACE GAME 2" );
    window.setFramerateLimit( 120 );

    sf::Sprite spr;
    sf::Texture tex, norm;
    // tex.loadFromFile("human_diff.png");
    tex.loadFromFile("brickwall.jpg");
    // norm.loadFromFile("human_norm.png");
    norm.loadFromFile("brickwall_normal.jpg");
    spr.setTexture(tex);
    spr.setScale(0.5,0.5);
    spr.setPosition( sf::Vector2f(400,300) );

    sf::Shader shader;

    // shader.loadFromFile( "vertex.vert", sf::Shader::Vertex );
    shader.loadFromFile( "fragment.frag", sf::Shader::Fragment );
    shader.setUniform("u_texture",tex);
    shader.setUniform("u_normals",norm);
    while(window.isOpen())
    {
        sf::Event event;
        while( window.pollEvent( event ) )
        {
            if(event.type == sf::Event::Closed) window.close();
        }
        shader.setUniform("LightPos", sf::Vector3f(
             sf::Mouse::getPosition( window ).x,
             sf::Mouse::getPosition( window ).y,
             1.0f
        ));
        window.clear( sf::Color::Black );
        window.draw( spr, &shader );
        window.display();
    }
}