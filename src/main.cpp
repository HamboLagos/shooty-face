#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "config.h"

#include "game.hpp"
#include "player.hpp"
#include "gun.hpp"
#include "bullet.hpp"
#include "barrier.hpp"
#include "enemy.hpp"

#include "components/physics.hpp"
#include "components/health.hpp"

using namespace std;

void add_renderings(Renderer::Renderings& to, const Renderer::Renderings& from)
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

    auto& game = Game::instance();

    sf::RenderWindow app(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Shooty Face");
    app.setFramerateLimit(250);

    auto& player = *game.add_player();
    player.get_component<Physics>()->set_position(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT)/2.f);

    game.entity_collection().push_back(std::make_unique<Gun>(player));
    Gun& gun = static_cast<Gun&>(*game.entity_collection().back().get());
    gun.set_ammunition(std::make_unique<BulletAmmunition>());

    std::vector<Barrier*> barriers(4); ///< top, left, bottom, right
    for(auto& barrier : barriers) {
        barrier = new Barrier();
        game.entity_collection().emplace_back(barrier);
    }

    float border_thickness = 20.f;
    barriers[0]->get_component<Physics>()->set_position({WINDOW_WIDTH/2.f, border_thickness/2.f});
    barriers[0]->get_component<Physics>()->set_dimensions({WINDOW_WIDTH, border_thickness});
    barriers[1]->get_component<Physics>()->set_position({border_thickness/2.f, WINDOW_HEIGHT/2.f});
    barriers[1]->get_component<Physics>()->set_dimensions({border_thickness, WINDOW_HEIGHT});
    barriers[2]->get_component<Physics>()->set_position({WINDOW_WIDTH/2.f, WINDOW_HEIGHT - border_thickness/2.f});
    barriers[2]->get_component<Physics>()->set_dimensions({WINDOW_WIDTH, border_thickness});
    barriers[3]->get_component<Physics>()->set_position({WINDOW_WIDTH - border_thickness/2.f, WINDOW_HEIGHT/2.f});
    barriers[3]->get_component<Physics>()->set_dimensions({border_thickness, WINDOW_HEIGHT});

    barriers.clear();

    std::vector<Enemy*> enemies(24);
    int ndx = 0;
    for(auto& enemy : enemies) {
        enemy = new Enemy();
        enemy->get_component<Health>()->set_health(15.f);

        /* auto* physics = enemy->get_component<Physics>(); */
        /* if (ndx++ <= 3) { */
        /*     physics->set_move_speed(physics->get_move_speed() * 2.f); */
        /* } */

        game.entity_collection().emplace_back(enemy);
    }


    sf::Vector2f zero_offset = {border_thickness + enemies[0]->get_component<Physics>()->get_extents().x,
                                border_thickness + enemies[0]->get_component<Physics>()->get_extents().y};
    enemies[0]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[1]->get_component<Physics>()->set_position({zero_offset.x, WINDOW_HEIGHT - zero_offset.y});
    enemies[2]->get_component<Physics>()->set_position({WINDOW_WIDTH - zero_offset.x, WINDOW_HEIGHT - zero_offset.y});
    enemies[3]->get_component<Physics>()->set_position({WINDOW_WIDTH - zero_offset.x, zero_offset.y});

    zero_offset = {WINDOW_WIDTH/2.f, WINDOW_HEIGHT/4.f};
    enemies[4]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/16.f, zero_offset.y});
    enemies[5]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[6]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[7]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[8]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/16.f, zero_offset.y});

    zero_offset = {WINDOW_WIDTH/4.f, WINDOW_HEIGHT/2.f};
    enemies[9]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/16.f});
    enemies[10]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/8.f});
    enemies[11]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[12]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/8.f});
    enemies[13]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/16.f});

    zero_offset = {WINDOW_WIDTH/2.f, WINDOW_HEIGHT*3.f/4.f};
    enemies[14]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/16.f, zero_offset.y});
    enemies[15]->get_component<Physics>()->set_position({zero_offset.x - WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[16]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[17]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/8.f, zero_offset.y});
    enemies[18]->get_component<Physics>()->set_position({zero_offset.x + WINDOW_WIDTH/16.f, zero_offset.y});

    zero_offset = {WINDOW_WIDTH*3.f/4.f, WINDOW_HEIGHT/2.f};
    enemies[19]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/16.f});
    enemies[20]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y - WINDOW_HEIGHT/8.f});
    enemies[21]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y});
    enemies[22]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/8.f});
    enemies[23]->get_component<Physics>()->set_position({zero_offset.x, zero_offset.y + WINDOW_HEIGHT/16.f});

    enemies.clear();

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
                gun.reload();
                break;

            default:
                break;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            auto target = sf::Mouse::getPosition(app);
            gun.fire(sf::Vector2f(target));
        }

        /* static int last_projectile_count = 0; */
        /* int projectile_count = player->get_gun().get_magazine().size(); */
        /* if (last_projectile_count != projectile_count) { */
        /*     last_projectile_count = projectile_count; */
        /*     std::cout << "Projectile Count: " << projectile_count << std::endl; */
        /* } */

        for(auto& entity : game.entities()) {
            entity->update(elapsed);
        }

        /* int entity_count = game.entities().size(); */

        auto& collection = game.entity_collection();
        collection.erase(
            std::remove_if(collection.begin(), collection.end(),
                           [](auto& entity) { return entity->is_dead(); }),
            collection.end());

        /* int delta_entities = entity_count - collection.size(); */

        /* if (delta_entities != 0) { */
        /*     std::cout << */
        /*         "Size: " << */
        /*         entity_count - delta_entities << " " << */
        /*         "Delta: " << */
        /*         delta_entities << */
        /*         std::endl; */
        /* } */

        Graphics::Renderings all_renderings;

        for(auto& entity : game.entities()) {
            if (entity->has_component<Graphics>()) {
                add_renderings(all_renderings, entity->get_component<Graphics>()->render());
            }
        }

        app.clear(sf::Color::White);
        for(const auto* rendering : all_renderings) {
            app.draw(*rendering);
        }
        app.display();
    }
    return 0;
}
