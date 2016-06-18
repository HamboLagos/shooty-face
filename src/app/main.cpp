#include "config.h"
#include <iostream>
#include <SFML/Graphics.hp>
using namespace std;

int main(int argc, char *argv[])
{
    std::cout << "Version " << shooty_face_VERSION_MAJOR << "." << shooty_face_VERSION_MINOR << std::endl;

    sf::Window app(sf::VideoMode(800, 600), "shooty_face");

    while(app.isOpen()) {
        sf::Event event;
        while(App.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                app.close();
            }
        }
        app.display();
    }
    return 0;
}
