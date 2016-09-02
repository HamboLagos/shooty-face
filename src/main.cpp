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
    player.set_position({400.f, 300.f});
    player.set_velocity({5.f, 10.f});
    player.set_projectile_speed(20.f);

    while (app.isOpen()) {

        sf::Event event;
        while (app.pollEvent(event)) {
            switch (event.type) {

            case sf::Event::Closed:
                app.close();
                break;

            case sf::Event::KeyReleased:

                switch (event.key.code) {
                case sf::Keyboard::Escape:
                    app.close();
                    break;
                case sf::Keyboard::W:
                    player.stop_move(Player::Direction::UP);
                    break;
                case sf::Keyboard::A:
                    player.stop_move(Player::Direction::LEFT);
                    break;
                case sf::Keyboard::S:
                    player.stop_move(Player::Direction::DOWN);
                    break;
                case sf::Keyboard::D:
                    player.stop_move(Player::Direction::RIGHT);
                    break;
                default:
                    break;
                }

                break;

            case sf::Event::KeyPressed:

                switch (event.key.code) {
                case sf::Keyboard::W:
                    player.start_move(Player::Direction::UP);
                    break;
                case sf::Keyboard::A:
                    player.start_move(Player::Direction::LEFT);
                    break;
                case sf::Keyboard::S:
                    player.start_move(Player::Direction::DOWN);
                    break;
                case sf::Keyboard::D:
                    player.start_move(Player::Direction::RIGHT);
                    break;
                default:
                    break;
                }

                break;

            case sf::Event::MouseButtonReleased:
                player.shoot(sf::Mouse::getPosition(app));
                break;

            default:
                break;
            }
        }

        player.update();

        app.clear(sf::Color::White);
        app.draw(player.render());
        auto* projectile = player.get_projectile();
        if (projectile != nullptr) {
            app.draw(projectile->render());
        }
        app.display();
    }
    return 0;
}
