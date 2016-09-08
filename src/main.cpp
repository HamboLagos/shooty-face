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

    static constexpr float WINDOW_WIDTH = 1200.f;
    static constexpr float WINDOW_HEIGHT = 1200.f;

    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shooty Face");
    app.setFramerateLimit(250);

    float border_thickness = 20.f;
    std::vector<Barrier> barriers(4); ///< top, left, bottom, right

    barriers[0].set_position({WINDOW_WIDTH/2.f, border_thickness/2.f});
    barriers[0].set_dimensions({WINDOW_WIDTH, border_thickness});

    barriers[1].set_position({border_thickness/2.f, WINDOW_HEIGHT/2.f});
    barriers[1].set_dimensions({border_thickness, WINDOW_HEIGHT});

    barriers[2].set_position({WINDOW_WIDTH/2.f, WINDOW_HEIGHT - border_thickness/2.f});
    barriers[2].set_dimensions({WINDOW_WIDTH, border_thickness});

    barriers[3].set_position({WINDOW_WIDTH - border_thickness/2.f, WINDOW_HEIGHT/2.f});
    barriers[3].set_dimensions({border_thickness, WINDOW_HEIGHT});

    Player player;
    player.set_dimensions({20.f, 20.f});
    player.set_position(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)/2.f);
    player.get_gun().set_ammunition(std::make_unique<Bullet>());

    std::vector<Enemy> enemies(24);
    int ndx = 0;
    for(auto& enemy : enemies) {
        if (ndx++ <= 3) {
            enemy.set_fast();
        } else {
            enemy.set_slow();
            enemy.set_health(15.f);
        }
    }

    sf::Vector2f zero_offset = {border_thickness + enemies[0].get_extents().x,
                                border_thickness + enemies[0].get_extents().y};
    enemies[0].set_position({zero_offset.x, zero_offset.y});
    enemies[1].set_position({zero_offset.x, WINDOW_HEIGHT - zero_offset.y});
    enemies[2].set_position({WINDOW_WIDTH - zero_offset.x, WINDOW_HEIGHT - zero_offset.y});
    enemies[3].set_position({WINDOW_WIDTH - zero_offset.x, zero_offset.y});

    zero_offset = {WINDOW_WIDTH/2.f, WINDOW_HEIGHT/4.f};
    enemies[4].set_position({zero_offset.x - WINDOW_WIDTH/16.f, zero_offset.y});
    enemies[5].set_position({zero_offset.x - WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[6].set_position({zero_offset.x, zero_offset.y});
    enemies[7].set_position({zero_offset.x + WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[8].set_position({zero_offset.x + WINDOW_WIDTH/16.f, zero_offset.y});

    zero_offset = {WINDOW_WIDTH/4.f, WINDOW_HEIGHT/2.f};
    enemies[9].set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/16.f});
    enemies[10].set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/8.f});
    enemies[11].set_position({zero_offset.x, zero_offset.y});
    enemies[12].set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/8.f});
    enemies[13].set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/16.f});

    zero_offset = {WINDOW_WIDTH/2.f, WINDOW_HEIGHT*3.f/4.f};
    enemies[14].set_position({zero_offset.x - WINDOW_WIDTH/16.f, zero_offset.y});
    enemies[15].set_position({zero_offset.x - WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[16].set_position({zero_offset.x, zero_offset.y});
    enemies[17].set_position({zero_offset.x + WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[18].set_position({zero_offset.x + WINDOW_WIDTH/16.f, zero_offset.y});

    zero_offset = {WINDOW_WIDTH*3.f/4.f, WINDOW_HEIGHT/2.f};
    enemies[19].set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/16.f});
    enemies[20].set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/8.f});
    enemies[21].set_position({zero_offset.x, zero_offset.y});
    enemies[22].set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/8.f});
    enemies[23].set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/16.f});

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
        }

        player.update(elapsed);

        for(auto& enemy : enemies) {
            enemy.set_destination(player.get_position());
            enemy.update(elapsed);
        }

        Collision collision;
        for(const auto& barrier : barriers) {
            if (collision.test(player.get_box(), barrier.get_box())) {
                player.move(collision.get_penetration());
            }

            for(auto& enemy : enemies) {
                if (collision.test(enemy.get_box(), barrier.get_box())) {
                    enemy.move(collision.get_penetration());
                }
            }

            for(auto& projectile : player.get_gun().get_magazine()) {
                if (collision.test(projectile->get_box(), barrier.get_box())) {
                    projectile->kill();
                }
            }
        }

        int ndx = 0;
        for(auto& enemy : enemies) {
            ++ndx;
            for (auto iter = enemies.begin() + ndx; iter != enemies.end(); ++iter) {
                if (collision.test(enemy.get_box(), iter->get_box())) {
                    std::cout << "Moving enemy: " << ndx << std::endl;
                    enemy.move(collision.get_penetration());
                }
            }
        }

        for(auto& projectile : player.get_gun().get_magazine()) {
            for(auto& enemy : enemies) {
                if (collision.test(projectile->get_box(), enemy.get_box())) {
                    enemy.damage(5.f);
                    projectile->kill();
                }
            }
        }

        for(auto& enemy : enemies) {
            if (collision.test(player.get_box(), enemy.get_box())) {
                player.move(collision.get_penetration());
            }
        }

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                           [](auto& enemy) { return enemy.is_dead(); }),
            enemies.end());

        Graphical::Renderings all_renderings;

        for(auto& barrier : barriers) {
            barrier.render();
            add_renderings(all_renderings, barrier.get_renderings());
        }

        player.render();
        add_renderings(all_renderings, player.get_renderings());

        for(auto& enemy : enemies) {
            enemy.render();
            add_renderings(all_renderings, enemy.get_renderings());
        }

        app.clear(sf::Color::White);
        for(const auto* rendering : all_renderings) {
            app.draw(*rendering);
        }
        app.display();
    }
    return 0;
}
