#include <config.h>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "collision.hpp"
#include "enemy.hpp"

using namespace std;
using Ammunition = Gun::Ammunition;

int main(int argc, char *argv[])
{
    std::cout << "Version " << shooty_face_VERSION_MAJOR
              << "."        << shooty_face_VERSION_MINOR
              << "."        << shooty_face_VERSION_REVIS
              << std::endl;

    static constexpr float WINDOW_WIDTH = 500.f;
    static constexpr float WINDOW_HEIGHT = 500.f;

    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shooty Face");
    app.setFramerateLimit(250);

    Player player;
    player.set_dimensions({20.f, 20.f});
    player.set_position({400.f, 300.f});
    player.set_velocity({100.f, 100.f});
    player.get_gun().set_ammunition(Ammunition::Bullet);

    Enemy enemy;
    enemy.set_dimensions({40.f, 40.f});
    enemy.set_position({50.f, 50.f});
    enemy.set_velocity({2.f, 2.f});

    sf::Vector2f dimensions = {WINDOW_WIDTH, WINDOW_HEIGHT};
    AABB top_wall({0 + dimensions.x/2, 0 - dimensions.y/2}, dimensions);
    AABB left_wall({0 - dimensions.x/2, 0 + dimensions.y/2}, dimensions);
    AABB bottom_wall(top_wall.get_origin() + sf::Vector2f(0.f, dimensions.y + WINDOW_HEIGHT), dimensions);
    AABB right_wall(left_wall.get_origin() + sf::Vector2f(dimensions.x + WINDOW_WIDTH, 0.f), dimensions);

    sf::Clock clock;
    while (app.isOpen()) {

        sf::Time elapsed = clock.restart();

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
                {
                    auto target = sf::Mouse::getPosition(app);
                    player.get_gun().fire(sf::Vector2f(target));

                    std::cout << "Projectile Count: " <<
                        player.get_gun().get_elements().size() << std::endl;
                }
                break;

            default:
                break;
            }
        }

        player.update(elapsed);
        enemy.update(elapsed);

        Collision collision;
        if (collision.test(player.get_box(), top_wall)) {
            player.move(collision.get_penetration());
        }
        if (collision.test(player.get_box(), left_wall)) {
            player.move(collision.get_penetration());
        }
        if (collision.test(player.get_box(), bottom_wall)) {
            player.move(collision.get_penetration());
        }
        if (collision.test(player.get_box(), right_wall)) {
            player.move(collision.get_penetration());
        }

        for(auto* projectile : player.get_gun().get_elements()) {
            if (collision.test(projectile->get_box(), top_wall) ||
                collision.test(projectile->get_box(), bottom_wall)) {
                auto velocity = projectile->get_velocity();
                projectile->set_velocity({velocity.x, -velocity.y});
            }
            if (collision.test(projectile->get_box(), left_wall) ||
                collision.test(projectile->get_box(), right_wall)) {
                auto velocity = projectile->get_velocity();
                projectile->set_velocity({-velocity.x, velocity.y});
            }
        }

        for(auto* projectile : player.get_gun().get_elements()) {
            if (collision.test(projectile->get_box(), enemy.get_box())) {
                projectile->kill();
                enemy.damage(10.f);

                if (enemy.is_dead()) {
                    enemy.heal(100.f); // reset health
                    enemy.set_velocity({0.f, 0.f});
                    enemy.animate();
                } else {
                    enemy.set_velocity(projectile->get_velocity()/2.f);
                    enemy.animate();
                }

                std::cout << "Projectile Count: " <<
                    player.get_gun().get_elements().size()-1 << std::endl;
            }
        }

        if (collision.test(enemy.get_box(), top_wall) ||
            collision.test(enemy.get_box(), bottom_wall)) {
            auto velocity = enemy.get_velocity();
            enemy.set_velocity({velocity.x, -velocity.y});
        }
        if (collision.test(enemy.get_box(), left_wall) ||
            collision.test(enemy.get_box(), right_wall)) {
            auto velocity = enemy.get_velocity();
            enemy.set_velocity({-velocity.x, velocity.y});
        }

        app.clear(sf::Color::White);
        app.draw(player.render());
        app.draw(enemy.render());
        for(const auto* rendering : player.get_gun().render()) {
            app.draw(*rendering);
        }
        app.display();
    }
    return 0;
}
