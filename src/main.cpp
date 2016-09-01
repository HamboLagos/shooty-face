#include <config.h>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "player.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    std::cout << "Version " << shooty_face_VERSION_MAJOR
              << "."        << shooty_face_VERSION_MINOR
              << "."        << shooty_face_VERSION_REVIS
              << std::endl;

    sf::RenderWindow app(sf::VideoMode(800, 600), "Shooty Face");
    app.setFramerateLimit(60);

    Player player;
    player.set_position({350, 250});

    while(app.isOpen()) {
        sf::Event event;
        while(app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                app.close();
            }
        }

        app.clear(sf::Color::White);
        app.draw(player.render());
        app.display();
    }
    return 0;
}
