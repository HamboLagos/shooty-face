#include <iostream>

#include <SFML/Graphics.hpp>

#include "barrier.hpp"
#include "bullet.hpp"
#include "collision.hpp"
#include "config.h"
#include "enemy.hpp"
#include "health_bar.hpp"
#include "player.hpp"

using namespace std;
using Renderings = Graphical::Renderings;

void add_renderings(Renderings& to, const Renderings& from)
{
    to.insert(to.end(), from.begin(), from.end());
}

int main(int argc, char *argv[])
{
    std::cout << "Version " << shooty_face_VERSION_MAJOR
              << "."        << shooty_face_VERSION_MINOR
              << "."        << shooty_face_VERSION_REVIS
              << std::endl;

    static constexpr float WINDOW_WIDTH = 2000.f;
    static constexpr float WINDOW_HEIGHT = 1000.f;

    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shooty Face");
    app.setFramerateLimit(250);

    Player player;
    player.set_dimensions({20.f, 20.f});
    player.set_position({400.f, 300.f});
    player.get_gun().set_ammunition(std::make_unique<Bullet>());

    Enemy enemy;
    enemy.set_dimensions({40.f, 40.f});
    enemy.set_position({50.f, 50.f});
    enemy.set_velocity({2.f, 2.f});
    enemy.set_health(50.f);

    sf::Vector2f dimensions = {WINDOW_WIDTH, WINDOW_HEIGHT};
    float border_thickness = 20.f;

    std::vector<Barrier> barriers(4); ///< top, left, bottom, right

    barriers[0].set_position({dimensions.x/2.f, border_thickness/2.f});
    barriers[0].set_dimensions({dimensions.x, border_thickness});

    barriers[1].set_position({border_thickness/2.f, dimensions.y/2.f});
    barriers[1].set_dimensions({border_thickness, dimensions.y});

    barriers[2].set_position({dimensions.x/2.f, dimensions.y - border_thickness/2.f});
    barriers[2].set_dimensions({dimensions.x, border_thickness});

    barriers[3].set_position({dimensions.x - border_thickness/2.f, dimensions.y/2.f});
    barriers[3].set_dimensions({border_thickness, dimensions.y});

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
                player.get_gun().reload();
                break;

            default:
                break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto target = sf::Mouse::getPosition(app);
            player.get_gun().fire(sf::Vector2f(target));
        }

        static int last_projectile_count = 0;
        int projectile_count = player.get_gun().get_magazine().size();
        if (last_projectile_count != projectile_count) {
            last_projectile_count = projectile_count;

            std::cout << "Projectile Count: " <<
                projectile_count << std::endl;
        }

        player.update(elapsed);
        enemy.update(elapsed);

        Collision collision;
        for(const auto& barrier : barriers) {
            if (collision.test(player.get_box(), barrier.get_box())) {
                player.move(collision.get_penetration());
            }

            if (collision.test(enemy.get_box(), barrier.get_box())) {
                enemy.move(collision.get_penetration());

                auto velocity = enemy.get_velocity();
                if (collision.get_penetration().x == 0.f) {
                    enemy.set_velocity({velocity.x, -velocity.y});
                } else {
                    enemy.set_velocity({-velocity.x, velocity.y});
                }
            }

            for(auto& projectile : player.get_gun().get_magazine()) {
                if (collision.test(projectile->get_box(), barrier.get_box())) {
                    projectile->kill();
                }
            }
        }

        for(auto& projectile : player.get_gun().get_magazine()) {
            if (collision.test(projectile->get_box(), enemy.get_box())) {
                enemy.damage(5.f);
                projectile->kill();

                if (enemy.is_alive()) {
                    enemy.set_velocity(projectile->get_velocity()/4.f);
                }
            }
        }

        Graphical::Renderings all_renderings;

        for(auto& barrier : barriers) {
            barrier.render();
            add_renderings(all_renderings, barrier.get_renderings());
        }

        player.render();
        add_renderings(all_renderings, player.get_renderings());

        enemy.render();
        add_renderings(all_renderings, enemy.get_renderings());

        app.clear(sf::Color::White);
        for(const auto* rendering : all_renderings) {
            app.draw(*rendering);
        }
        app.display();
    }
    return 0;
}
