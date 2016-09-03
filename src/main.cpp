#include <config.h>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "collision.hpp"
#include "enemy.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    std::cout << "Version " << shooty_face_VERSION_MAJOR
              << "."        << shooty_face_VERSION_MINOR
              << "."        << shooty_face_VERSION_REVIS
              << std::endl;

    static constexpr float WINDOW_WIDTH = 500.f;
    static constexpr float WINDOW_HEIGHT = 500.f;

    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shooty Face");
    app.setFramerateLimit(60);

    Player player;
    player.set_position({400.f, 300.f});
    player.set_move_velocity({10.f, 10.f});
    player.set_projectile_speed(20.f);

    Enemy enemy;
    enemy.set_position({50.f, 50.f});
    enemy.set_move_velocity({2.f, 2.f});

    sf::Vector2f dimensions = {WINDOW_WIDTH, WINDOW_HEIGHT};
    AABB top_wall({0 + dimensions.x/2, 0 - dimensions.y/2}, dimensions);
    AABB left_wall({0 - dimensions.x/2, 0 + dimensions.y/2}, dimensions);
    AABB bottom_wall(top_wall.get_origin() + sf::Vector2f(0.f, dimensions.y + WINDOW_HEIGHT), dimensions);
    AABB right_wall(left_wall.get_origin() + sf::Vector2f(dimensions.x + WINDOW_WIDTH, 0.f), dimensions);

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

        Collision collision;
        if (collision.test(player.get_AABB(), top_wall)) {
            player.move(collision.get_penetration());
        }
        if (collision.test(player.get_AABB(), left_wall)) {
            player.move(collision.get_penetration());
        }
        if (collision.test(player.get_AABB(), bottom_wall)) {
            player.move(collision.get_penetration());
        }
        if (collision.test(player.get_AABB(), right_wall)) {
            player.move(collision.get_penetration());
        }

        auto* projectile = player.get_projectile();
        if (projectile != nullptr) {
            if (collision.test(projectile->get_AABB(), top_wall) ||
                collision.test(projectile->get_AABB(), bottom_wall)) {
                auto velocity = projectile->get_velocity();
                projectile->set_velocity({velocity.x, -velocity.y});
            }
            if (collision.test(projectile->get_AABB(), left_wall) ||
                collision.test(projectile->get_AABB(), right_wall)) {
                auto velocity = projectile->get_velocity();
                projectile->set_velocity({-velocity.x, velocity.y});
            }
        }

        enemy.update();
        if (projectile != nullptr) {
            if (collision.test(projectile->get_AABB(), enemy.get_AABB())) {
                projectile->kill();
                if (enemy.is_running()) {
                    enemy.stop();
                } else {
                    enemy.set_move_velocity(projectile->get_velocity()/4.f);
                    enemy.start();
                }
            }
        }

        if (collision.test(enemy.get_AABB(), top_wall) ||
            collision.test(enemy.get_AABB(), bottom_wall)) {
            auto velocity = enemy.get_move_velocity();
            enemy.set_move_velocity({velocity.x, -velocity.y});
        }
        if (collision.test(enemy.get_AABB(), left_wall) ||
            collision.test(enemy.get_AABB(), right_wall)) {
            auto velocity = enemy.get_move_velocity();
            enemy.set_move_velocity({-velocity.x, velocity.y});
        }

        app.clear(sf::Color::White);
        app.draw(player.render());
        if (projectile != nullptr) {
            app.draw(projectile->render());
        }
        app.draw(enemy.render());
        app.display();
    }
    return 0;
}
